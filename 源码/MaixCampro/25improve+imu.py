from enum import auto
from maix import camera, display, image, nn, app, time, pinmap, gpio, uart, ahrs
import cv2
import numpy as np
import math
from maix.ext_dev import imu

#######################################
# 1. Mahony滤波参数
kp = 2
ki = 0.01
calibrate_first = False

# 2. IMU初始化
imu_sensor = imu.IMU(
    "qmi8658", mode=imu.Mode.DUAL,
    acc_scale=imu.AccScale.ACC_SCALE_2G,
    acc_odr=imu.AccOdr.ACC_ODR_1000,
    gyro_scale=imu.GyroScale.GYRO_SCALE_256DPS,
    gyro_odr=imu.GyroOdr.GYRO_ODR_8000
)
ahrs_filter = ahrs.MahonyAHRS(kp, ki)

# 3. IMU校准
if calibrate_first or not imu_sensor.calib_gyro_exists():
    # print("\n\nNeed calibrate first")
    # print("now calibrate, please !! don't move !! device, wait for 10 seconds")
    imu_sensor.calib_gyro(10000)
else:
    imu_sensor.load_calib_gyro()
#######################################

# --------- 视觉循迹参数 ----------
debug_draw_err_line = True
crop_padding = 12
rect_min_limit = 12
std_from_white_rect = True
circle_num_points = 50
std_res = [int(29.7 / 21 * 80), 80]
cam_buff_num = 1
auto_awb = True
contrast = 80

# --------- PID+IMU参数 ----------
P_STRAIGHT = 1
P_TURN = 1.15
DEADZONE = 3
GYRO_THRESHOLD = 0.4
def is_turning(gyro_z, threshold=GYRO_THRESHOLD):
    return abs(gyro_z) > threshold

# --------- 硬件初始化 ----------
pinmap.set_pin_function("A16", "UART0_TX")
pinmap.set_pin_function("A17", "UART0_RX")
serial = uart.UART("/dev/ttyS0", 115200)
pinmap.set_pin_function("A19", "GPIOA19")
led = gpio.GPIO("GPIOA19", gpio.Mode.OUT)

detector = nn.YOLOv5(model="/root/models/model_3356.mud", dual_buff=True)
cam = camera.Camera(detector.input_width(), detector.input_height(), detector.input_format(), buff_num=cam_buff_num)
if not auto_awb:
    cam.awb_mode(camera.AwbMode.Manual)
    cam.set_wb_gain(awb_gain)
cam.constrast(contrast)

disp = display.Display()

def uart_update(a, b):
    # 帧头
    header = bytes([0xFF, 0xFE])
    import struct
    data_bytes = struct.pack('<ff', float(a), float(b))
    # 校验
    block_check = 0
    for byte in data_bytes:
        block_check ^= byte
    packet = header + data_bytes + bytes([block_check])
    serial.write(packet)
    # print(f"Sent: {packet}")

# --------- 主循环 ----------
laser_pos = [cam.width() // 2 - 15, cam.height() // 3 + 20 ]
last_center = laser_pos
last_center_small = [detector.input_width(), detector.input_height()]
led.value(1)

last_time = time.ticks_s()
while not app.need_exit():
    img = cam.read()
    img_ai = img

    objs = detector.detect(img_ai, conf_th=0.5, iou_th=0.45)
    max_idx = -1
    max_s = 0
    for i, obj in enumerate(objs):
        s = obj.w * obj.h
        if s > max_s:
            max_s = s
            max_idx = i

    if max_idx >= 0:
        obj = objs[max_idx]
        w = obj.w + crop_padding * 2
        h = obj.h + crop_padding * 2
        w = w + 1 if w % 2 != 0 else w
        h = h + 1 if h % 2 != 0 else h
        x = obj.x - crop_padding
        y = obj.y - crop_padding
        if x < 0:
            w += x
            x = 0
        if y < 0:
            h += y
            y = 0
        if x + w > img_ai.width():
            w = img_ai.width() - x
        if y + h > img_ai.height():
            h = img_ai.height() - y
        crop_ai = img_ai.crop(x, y, w, h)
        crop_ai_rect = [x, y, w, h]
        img_ai_scale = [img.width() / img_ai.width(), img.height() / img_ai.height()]
        img_cv = image.image2cv(img, False, False)
        gray = crop_ai.to_format(image.Format.FMT_GRAYSCALE)
        gray_cv = image.image2cv(gray, False, False)
        blurred = cv2.GaussianBlur(gray_cv, (3, 3), 0)
        binary = cv2.adaptiveThreshold(blurred, 255,
                       cv2.ADAPTIVE_THRESH_MEAN_C,
                       cv2.THRESH_BINARY_INV, 27, 31)
        if std_from_white_rect:
            h, w_ = binary.shape[:2]
            mask = np.zeros((h + 2, w_ + 2), np.uint8)
            cv2.floodFill(binary, mask, (2, 2), 255, loDiff=5, upDiff=5, flags=4)
            cv2.floodFill(binary, mask, (w_ - 2, h - 2), 255, loDiff=5, upDiff=5, flags=4)
            binary = cv2.bitwise_not(binary)

        contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if len(contours) > 0:
            largest_contour = max(contours, key=cv2.contourArea)
            epsilon = 0.02 * cv2.arcLength(largest_contour, True)
            approx = cv2.approxPolyDP(largest_contour, epsilon, True)
            if len(approx) == 4:
                corners = approx.reshape((4, 2))
                rect = np.zeros((4, 2), dtype="float32")
                s = corners.sum(axis=1)
                rect[0] = corners[np.argmin(s)] # 左上
                rect[2] = corners[np.argmax(s)] # 右下
                diff = np.diff(corners, axis=1)
                rect[3] = corners[np.argmax(diff)] # 左下
                rect[1] = corners[np.argmin(diff)] # 右上
                minW = min(rect[1][0] - rect[0][0], rect[2][0] - rect[3][0])
                minH = min(rect[3][1] - rect[0][1], rect[2][1] - rect[1][1])
                if minH > rect_min_limit and minW > rect_min_limit:
                    maxWidth = std_res[0]
                    maxHeight = std_res[1]
                    rect[:, 0] += crop_ai_rect[0]
                    rect[:, 1] += crop_ai_rect[1]
                    rect[:, 0] *= img_ai_scale[0]
                    rect[:, 1] *= img_ai_scale[1]
                    dst = np.array([
                        [0, 0],
                        [maxWidth - 1, 0],
                        [maxWidth - 1, maxHeight - 1],
                        [0, maxHeight - 1]], dtype="float32")
                    M = cv2.getPerspectiveTransform(rect, dst)
                    M_inv = np.linalg.inv(M)
                    img_std_cv = cv2.warpPerspective(img_cv, M, (maxWidth, maxHeight))                
                    img_std = image.cv2image(img_std_cv, False, False)
                    if std_from_white_rect:
                        circle_dist = int(img_std.height() * 0.1149425287356322)
                    else:
                        circle_dist = img_std.height() * 0.09523809523809523
                    if circle_dist > 0:
                        center = [img_std.width() // 2, img_std.height() // 2]
                        std_center_points = np.array([[center]], dtype=np.float32)
                        original_center_point = cv2.perspectiveTransform(std_center_points, M_inv)[0][0].astype(np.int32).tolist()
                        err_center = [
                            original_center_point[0] - laser_pos[0],
                            original_center_point[1] - laser_pos[1],
                        ]
                       
                        # ---------  IMU数据获取与Mahony融合 ---------
                        data = imu_sensor.read_all(calib_gryo=True, radian=True)
                        t = time.ticks_s()
                        dt = t - last_time
                        last_time = t
                        angle = ahrs_filter.get_angle(data.acc, data.gyro, data.mag, dt, radian=False)
                        gyro_z = data.gyro.z # 单位：rad/s，拐角时绝对值较大

                        # --------- 动态P+死区PID ----------
                        if is_turning(gyro_z):
                            P_now = P_TURN
                        else:
                            P_now = P_STRAIGHT
                        if abs(err_center[0]) < DEADZONE:
                            err_x = 0.0
                        else:
                            err_x = err_center[0]
                        pid_input_x = err_x * P_now
                        # --------- 串口输出 ----------
                        uart_update(pid_input_x, err_center[1])

                        dist = math.hypot(original_center_point[0] - laser_pos[0], original_center_point[1] - laser_pos[1])
                        if dist < 5 :
                            led.value(0)
                        last_center = original_center_point
                        last_center_small = [int(last_center[0] / img_ai_scale[0]), int(last_center[1] / img_ai_scale[1])]
                    else:
                        print("detected circle too small", img_std.width(), img_std.height())
                else:
                    print(minW, minH, "rect not valid")
    # disp.show(img_ai)

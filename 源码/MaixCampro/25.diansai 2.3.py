from enum import auto
from maix import camera, display, image, nn, app, time, pinmap, gpio, uart, ahrs
import cv2
import numpy as np
import math
from maix.ext_dev import imu

#################################################################################

debug_draw_err_line = True   # 画出圆心和画面中心的误差线，需要消耗1ms左右时间
crop_padding = 12            # 裁切图时的外扩距离，调试到保证最近和最远位置整个黑框在检测框里，可以打开 DEBUG 模式看
rect_min_limit = 12          # 找到的大黑边框四个点最小距离必须大于这个值才有效，防止找到错误的值，可以放到最远位置测试
std_from_white_rect = True   # 裁切标准图是裁切自A4纸内部白色部分（更精准），False则是带黑框的外围框（整个A4纸）（更快一点点）
circle_num_points = 50       # 生成的第三个圆圈的点数量，控制圆边的平滑程度，可以用来巡迹
std_res = [int(29.7 / 21 * 80), 80]        # 找中心点和圆圈的分辨率，越大越精确，更慢，A4 29.7 x 21cm

cam_buff_num = 1             # 摄像头缓冲， 1 延迟更低帧率慢一点点， 2延迟更高帧率高一点点

auto_awb = True                            # 自动白平衡
contrast = 80                               # 对比度，阴影和圆圈痕迹都会更重
###################################################################################
# 初始化引脚
pinmap.set_pin_function("A16", "UART0_TX")
pinmap.set_pin_function("A17", "UART0_RX")
serial = uart.UART("/dev/ttyS0", 115200)
pinmap.set_pin_function("A19", "GPIOA19")
led = gpio.GPIO("GPIOA19", gpio.Mode.OUT)

# 初始化摄像头
detector = nn.YOLOv5(model="/root/models/model_3356.mud", dual_buff = True)
cam = camera.Camera(detector.input_width(), detector.input_height(), detector.input_format(), buff_num=cam_buff_num)
if not auto_awb:
    cam.awb_mode(camera.AwbMode.Manual)	
    cam.set_wb_gain(awb_gain)
cam.constrast(contrast)

disp = display.Display()

###########################################################################

def uart_update(a,b):
    # 帧头
    header = bytes([0xFF, 0xFE])
    # 将浮点数转换为小端字节序列
    import struct
    data_bytes = struct.pack('<ff', float(a), float(b))  # '<ff' 表示两个小端浮点数
    
    # 计算校验位（所有数据字节的异或）
    block_check = 0
    for byte in data_bytes:
        block_check ^= byte
    
    # 构建完整数据包: 帧头 + 浮点数字节 + 校验位
    packet = header + data_bytes + bytes([block_check])
    
    # 通过串口发送
    serial.write(packet)
    # print(f"Sent: {packet}")

###########################################################################
laser_pos = [164, 128]
last_center = laser_pos # 上一次检测到的圆心距离
last_center_small = [detector.input_width(), detector.input_height()] # 高清模式时，在小图的中心坐标
led.value(1)

start_flag=0#pid
rule_flag3=0
scan_flag=0
find_flag=0

kp = 2
ki = 0.01
calibrate_first = False
sensor = imu.IMU("qmi8658", mode=imu.Mode.DUAL,
                                    acc_scale=imu.AccScale.ACC_SCALE_2G,
                                    acc_odr=imu.AccOdr.ACC_ODR_1000,
                                    gyro_scale=imu.GyroScale.GYRO_SCALE_256DPS,
                                    gyro_odr=imu.GyroOdr.GYRO_ODR_8000)
ahrs_filter = ahrs.MahonyAHRS(kp, ki)
if calibrate_first or not sensor.calib_gyro_exists():
    sensor.calib_gyro(10000)
else:
    sensor.load_calib_gyro()

last_time = time.ticks_s()

while not app.need_exit():
################################  imu  #####################################################
    # get imu data
    anlge_data = sensor.read_all(calib_gryo = True, radian = True)
    # calculate angles
    t = time.ticks_s()
    dt = t - last_time
    last_time = t
    # print(f"{data.mag.x:8.2f}, {data.mag.y:8.2f}, {data.mag.z:8.2f}, {data.gyro.z:8.2f}")
    angle = ahrs_filter.get_angle(anlge_data.acc, anlge_data.gyro, anlge_data.mag, dt, radian = False)
#####################################################################################   
    data = serial.read()
    print(data)
    if data==b'02':# rule 2
        start_flag=1
        find_flag=0
    if  data==b'03':#rule 3
        rule_flag3=1
        find_flag=0
    if data==b'04':#stop
        start_flag=0
    if rule_flag3==1:
        error_x=angle.z-0
        error_y=angle.x-100
        uart_update(error_x,error_y)
        if find_flag==1:
            rule_flag3=0
            start_flag=1
        if error_x<2 and error_y<2:
            rule_flag3=0
            scan_flag=1
    if scan_flag==1:
        uart_update(25,0)
        if find_flag==1:
            scan_flag=0
            start_flag=1

    img = cam.read()
    # AI 检测外框
    img_ai = img # new copy
    objs = detector.detect(img_ai, conf_th = 0.5, iou_th = 0.45)
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
        # 算出裁切范围对应在大图的位置
        # 注意这里只考虑到了拉伸缩放(iamge.Fit.FILL)
        img_ai_scale = [img.width() / img_ai.width(), img.height() / img_ai.height()]
        # crop_rect = image.resize_map_pos_reverse(img.width(), img.height(), img_ai.width(), img_ai.height(), image.Fit.FIT_FILL, obj.x, obj.y, obj.w, obj.h)
        crop_rect = [int(obj.x * img_ai_scale[0]), int(obj.y * img_ai_scale[1]), int(obj.w * img_ai_scale[0]), int(h * img_ai_scale[0])]
        img_cv = image.image2cv(img, False, False)
        crop_ai_cv = image.image2cv(crop_ai, False, False)

        gray = crop_ai.to_format(image.Format.FMT_GRAYSCALE)
        gray_cv = image.image2cv(gray, False, False)
        # 高斯模糊去噪声
        blurred = cv2.GaussianBlur(gray_cv, (5, 5), 0)
        # 自适应二值化,找出黑色外轮廓,最后两个参数可以调整
        binary = cv2.adaptiveThreshold(blurred, 255,
                       cv2.ADAPTIVE_THRESH_MEAN_C,
                       cv2.THRESH_BINARY_INV, 27, 31)


        if std_from_white_rect:
            # 执行洪泛填充找出内白色轮廓
            h, w = binary.shape[:2]
            mask = np.zeros((h + 2, w + 2), np.uint8)
            # 设置种子点（左上角和右下角），如果环境好，可以只点一个角
            seed_point = (2, 2)
            seed_point2 = (w - 2, h - 2)
            # 设置填充值（白色 255）
            fill_value = 255
            # 执行洪泛填充（以左上角像素值为基准）
            cv2.floodFill(binary, mask, seed_point, fill_value, loDiff=5, upDiff=5, flags=4)
            cv2.floodFill(binary, mask, seed_point2, fill_value, loDiff=5, upDiff=5, flags=4)
            binary = cv2.bitwise_not(binary)

        # 查找轮廓4个角点
        approx = None
        contours, _ = cv2.findContours(binary, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if len(contours) > 0:
            # 筛选出最大的轮廓
            largest_contour = max(contours, key=cv2.contourArea)
            # 近似多边形
            epsilon = 0.02 * cv2.arcLength(largest_contour, True)
            approx = cv2.approxPolyDP(largest_contour, epsilon, True)
            # 如果找到的是一个四边形
            if len(approx) == 4:
                find_flag=1
                # 获取矩形四个角点
                # 对角点进行排序：左上、右上、右下、左下
                corners = approx.reshape((4, 2))
                # 按顺序排列角点（左上、右上、右下、左下）
                rect = np.zeros((4, 2), dtype="float32")
                s = corners.sum(axis=1)
                rect[0] = corners[np.argmin(s)] # 最小和，左上
                rect[2] = corners[np.argmax(s)] # 最大和，右下
                diff = np.diff(corners, axis=1) # y - x
                rect[3] = corners[np.argmax(diff)] # 差最大，左下
                rect[1] = corners[np.argmin(diff)] # 差最小，右上
                minW = min(rect[1][0] - rect[0][0], rect[2][0] - rect[3][0])
                minH = min(rect[3][1] - rect[0][1], rect[2][1] - rect[1][1])
                if minH > rect_min_limit and minW > rect_min_limit:
                    maxWidth = std_res[0]
                    maxHeight = std_res[1]

                    # rect 映射到大图, 从大图中得到标准内框图
                    rect[:, 0] += crop_ai_rect[0]
                    rect[:, 1] += crop_ai_rect[1]
                    rect[:, 0] *= img_ai_scale[0]
                    rect[:, 1] *= img_ai_scale[1]
                    # 透视变换
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

                        # 画圆中心
                        std_center_points = np.array([[center]], dtype=np.float32)
                        original_center_point = cv2.perspectiveTransform(std_center_points, M_inv)[0][0].astype(np.int32).tolist()
                        err_center = [
                            original_center_point[0] - laser_pos[0],
                            original_center_point[1] - laser_pos[1],
                        ]

                        img_ai.draw_circle(laser_pos[0], laser_pos[1], 3, image.COLOR_GREEN, thickness=-1)
                        img_ai.draw_circle(original_center_point[0], original_center_point[1], 3, image.COLOR_RED, thickness=-1)

                        if start_flag==1:
                            uart_update(err_center[0],err_center[1])

                        dist = math.hypot(original_center_point[0] - laser_pos[0], original_center_point[1] - laser_pos[1])
                        if dist < 3 and start_flag==1:
                            led.value(0)
                        else:
                            led.value(1)
                        # print(led.value())


                        last_center = original_center_point
                        last_center_small = [int(last_center[0] / img_ai_scale[0]), int(last_center[1] / img_ai_scale[1])]

                    else:
                        print("detected circle too small", img_std.width(), img_std.height())
                else:
                    print(minW, minH, "rect not valid")
        
    disp.show(img_ai)

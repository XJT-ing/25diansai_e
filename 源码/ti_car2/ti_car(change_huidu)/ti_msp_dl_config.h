/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for PWM */
#define PWM_INST                                                           TIMG0
#define PWM_INST_IRQHandler                                     TIMG0_IRQHandler
#define PWM_INST_INT_IRQN                                       (TIMG0_INT_IRQn)
#define PWM_INST_CLK_FREQ                                               40000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_C0_PORT                                                   GPIOA
#define GPIO_PWM_C0_PIN                                           DL_GPIO_PIN_12
#define GPIO_PWM_C0_IOMUX                                        (IOMUX_PINCM34)
#define GPIO_PWM_C0_IOMUX_FUNC                       IOMUX_PINCM34_PF_TIMG0_CCP0
#define GPIO_PWM_C0_IDX                                      DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_C1_PORT                                                   GPIOA
#define GPIO_PWM_C1_PIN                                           DL_GPIO_PIN_13
#define GPIO_PWM_C1_IOMUX                                        (IOMUX_PINCM35)
#define GPIO_PWM_C1_IOMUX_FUNC                       IOMUX_PINCM35_PF_TIMG0_CCP1
#define GPIO_PWM_C1_IDX                                      DL_TIMER_CC_1_INDEX



/* Defines for TIME_End */
#define TIME_End_INST                                                    (TIMG6)
#define TIME_End_INST_IRQHandler                                TIMG6_IRQHandler
#define TIME_End_INST_INT_IRQN                                  (TIMG6_INT_IRQn)
#define TIME_End_INST_LOAD_VALUE                                        (49999U)
/* Defines for Huidu_Time */
#define Huidu_Time_INST                                                  (TIMG7)
#define Huidu_Time_INST_IRQHandler                              TIMG7_IRQHandler
#define Huidu_Time_INST_INT_IRQN                                (TIMG7_INT_IRQn)
#define Huidu_Time_INST_LOAD_VALUE                                      (19999U)
/* Defines for Pid_Time */
#define Pid_Time_INST                                                    (TIMG8)
#define Pid_Time_INST_IRQHandler                                TIMG8_IRQHandler
#define Pid_Time_INST_INT_IRQN                                  (TIMG8_INT_IRQn)
#define Pid_Time_INST_LOAD_VALUE                                          (780U)




/* Defines for I2C_0 */
#define I2C_0_INST                                                          I2C0
#define I2C_0_INST_IRQHandler                                    I2C0_IRQHandler
#define I2C_0_INST_INT_IRQN                                        I2C0_INT_IRQn
#define GPIO_I2C_0_SDA_PORT                                                GPIOA
#define GPIO_I2C_0_SDA_PIN                                         DL_GPIO_PIN_0
#define GPIO_I2C_0_IOMUX_SDA                                      (IOMUX_PINCM1)
#define GPIO_I2C_0_IOMUX_SDA_FUNC                       IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_0_SCL_PORT                                                GPIOA
#define GPIO_I2C_0_SCL_PIN                                         DL_GPIO_PIN_1
#define GPIO_I2C_0_IOMUX_SCL                                      (IOMUX_PINCM2)
#define GPIO_I2C_0_IOMUX_SCL_FUNC                       IOMUX_PINCM2_PF_I2C0_SCL



/* Defines for ADC_VOLTAGE */
#define ADC_VOLTAGE_INST                                                    ADC0
#define ADC_VOLTAGE_INST_IRQHandler                              ADC0_IRQHandler
#define ADC_VOLTAGE_INST_INT_IRQN                                (ADC0_INT_IRQn)
#define ADC_VOLTAGE_ADCMEM_0                                  DL_ADC12_MEM_IDX_0
#define ADC_VOLTAGE_ADCMEM_0_REF                 DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADC_VOLTAGE_ADCMEM_0_REF_VOLTAGE_V                                     3.3
#define GPIO_ADC_VOLTAGE_C0_PORT                                           GPIOA
#define GPIO_ADC_VOLTAGE_C0_PIN                                   DL_GPIO_PIN_27

/* Defines for GRAY_OUT */
#define GRAY_OUT_INST                                                       ADC1
#define GRAY_OUT_INST_IRQHandler                                 ADC1_IRQHandler
#define GRAY_OUT_INST_INT_IRQN                                   (ADC1_INT_IRQn)
#define GRAY_OUT_ADCMEM_ADC_CH4                               DL_ADC12_MEM_IDX_0
#define GRAY_OUT_ADCMEM_ADC_CH4_REF              DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define GRAY_OUT_ADCMEM_ADC_CH4_REF_VOLTAGE_V                                     3.3
#define GPIO_GRAY_OUT_C4_PORT                                              GPIOB
#define GPIO_GRAY_OUT_C4_PIN                                      DL_GPIO_PIN_17



/* Port definition for Pin Group USERLED */
#define USERLED_PORT                                                     (GPIOB)

/* Defines for GREEN: GPIOB.22 with pinCMx 50 on package pin 21 */
#define USERLED_GREEN_PIN                                       (DL_GPIO_PIN_22)
#define USERLED_GREEN_IOMUX                                      (IOMUX_PINCM50)
/* Port definition for Pin Group OLED */
#define OLED_PORT                                                        (GPIOB)

/* Defines for SCL: GPIOB.9 with pinCMx 26 on package pin 61 */
#define OLED_SCL_PIN                                             (DL_GPIO_PIN_9)
#define OLED_SCL_IOMUX                                           (IOMUX_PINCM26)
/* Defines for SDA: GPIOB.8 with pinCMx 25 on package pin 60 */
#define OLED_SDA_PIN                                             (DL_GPIO_PIN_8)
#define OLED_SDA_IOMUX                                           (IOMUX_PINCM25)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOB)

/* Defines for KEY1: GPIOB.20 with pinCMx 48 on package pin 19 */
#define KEY_KEY1_PIN                                            (DL_GPIO_PIN_20)
#define KEY_KEY1_IOMUX                                           (IOMUX_PINCM48)
/* Defines for KEY2: GPIOB.21 with pinCMx 49 on package pin 20 */
#define KEY_KEY2_PIN                                            (DL_GPIO_PIN_21)
#define KEY_KEY2_IOMUX                                           (IOMUX_PINCM49)
/* Defines for KEY4: GPIOB.14 with pinCMx 31 on package pin 2 */
#define KEY_KEY4_PIN                                            (DL_GPIO_PIN_14)
#define KEY_KEY4_IOMUX                                           (IOMUX_PINCM31)
/* Port definition for Pin Group TB6612 */
#define TB6612_PORT                                                      (GPIOA)

/* Defines for AIN1: GPIOA.15 with pinCMx 37 on package pin 8 */
#define TB6612_AIN1_PIN                                         (DL_GPIO_PIN_15)
#define TB6612_AIN1_IOMUX                                        (IOMUX_PINCM37)
/* Defines for AIN2: GPIOA.14 with pinCMx 36 on package pin 7 */
#define TB6612_AIN2_PIN                                         (DL_GPIO_PIN_14)
#define TB6612_AIN2_IOMUX                                        (IOMUX_PINCM36)
/* Defines for BIN1: GPIOA.7 with pinCMx 14 on package pin 49 */
#define TB6612_BIN1_PIN                                          (DL_GPIO_PIN_7)
#define TB6612_BIN1_IOMUX                                        (IOMUX_PINCM14)
/* Defines for BIN2: GPIOA.16 with pinCMx 38 on package pin 9 */
#define TB6612_BIN2_PIN                                         (DL_GPIO_PIN_16)
#define TB6612_BIN2_IOMUX                                        (IOMUX_PINCM38)
/* Port definition for Pin Group EncoderA */
#define EncoderA_PORT                                                    (GPIOB)

/* Defines for PIN1: GPIOB.18 with pinCMx 44 on package pin 15 */
// pins affected by this interrupt request:["PIN1"]
#define EncoderA_INT_IRQN                                       (GPIOB_INT_IRQn)
#define EncoderA_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define EncoderA_PIN1_IIDX                                  (DL_GPIO_IIDX_DIO18)
#define EncoderA_PIN1_PIN                                       (DL_GPIO_PIN_18)
#define EncoderA_PIN1_IOMUX                                      (IOMUX_PINCM44)
/* Defines for PIN2: GPIOB.19 with pinCMx 45 on package pin 16 */
#define EncoderA_PIN2_PIN                                       (DL_GPIO_PIN_19)
#define EncoderA_PIN2_IOMUX                                      (IOMUX_PINCM45)
/* Port definition for Pin Group EncoderB */
#define EncoderB_PORT                                                    (GPIOA)

/* Defines for PIN3: GPIOA.17 with pinCMx 39 on package pin 10 */
// pins affected by this interrupt request:["PIN3"]
#define EncoderB_INT_IRQN                                       (GPIOA_INT_IRQn)
#define EncoderB_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define EncoderB_PIN3_IIDX                                  (DL_GPIO_IIDX_DIO17)
#define EncoderB_PIN3_PIN                                       (DL_GPIO_PIN_17)
#define EncoderB_PIN3_IOMUX                                      (IOMUX_PINCM39)
/* Defines for PIN4: GPIOA.24 with pinCMx 54 on package pin 25 */
#define EncoderB_PIN4_PIN                                       (DL_GPIO_PIN_24)
#define EncoderB_PIN4_IOMUX                                      (IOMUX_PINCM54)
/* Defines for GRAY_AD0: GPIOB.15 with pinCMx 32 on package pin 3 */
#define ganwei_huidu_GRAY_AD0_PORT                                       (GPIOB)
#define ganwei_huidu_GRAY_AD0_PIN                               (DL_GPIO_PIN_15)
#define ganwei_huidu_GRAY_AD0_IOMUX                              (IOMUX_PINCM32)
/* Defines for GRAY_AD1: GPIOB.16 with pinCMx 33 on package pin 4 */
#define ganwei_huidu_GRAY_AD1_PORT                                       (GPIOB)
#define ganwei_huidu_GRAY_AD1_PIN                               (DL_GPIO_PIN_16)
#define ganwei_huidu_GRAY_AD1_IOMUX                              (IOMUX_PINCM33)
/* Defines for GRAY_AD2: GPIOA.18 with pinCMx 40 on package pin 11 */
#define ganwei_huidu_GRAY_AD2_PORT                                       (GPIOA)
#define ganwei_huidu_GRAY_AD2_PIN                               (DL_GPIO_PIN_18)
#define ganwei_huidu_GRAY_AD2_IOMUX                              (IOMUX_PINCM40)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_init(void);
void SYSCFG_DL_TIME_End_init(void);
void SYSCFG_DL_Huidu_Time_init(void);
void SYSCFG_DL_Pid_Time_init(void);
void SYSCFG_DL_I2C_0_init(void);
void SYSCFG_DL_ADC_VOLTAGE_init(void);
void SYSCFG_DL_GRAY_OUT_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */

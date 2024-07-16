 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Omar Talaat
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                 (STD_ON)

/* Pre-compile option for Set Pin Direction API */
#define PORT_SET_PIN_DIRECTION_API              (STD_ON)

/* Pre-compile option for Set Pin Mode API */
#define PORT_SET_PIN_MODE_API                   (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                   (STD_ON)

/* Number of Pins in MCU */
#define PORT_NUMBER_OF_PINS                (43U)

/* Number of Ports defined */
#define PORT_NUMBER_OF_PORTS                    (6U)

/* Number needed to unlock the lock register in some pins */
#define PORT_LOCK_REG_NUM                       (0x4C4F434B)

/* Modes of Pins to be used in the Structure in Port_PBcfg.c */
#define PORTConf_PIN_MODE_ADC                       (Port_PinModeType)0
#define PORTConf_PIN_MODE_CAN                       (Port_PinModeType)1
#define PORTConf_PIN_MODE_DIO                       (Port_PinModeType)2
#define PORTConf_PIN_MODE_DIO_GPT                   (Port_PinModeType)3
#define PORTConf_PIN_MODE_DIO_WDG                   (Port_PinModeType)4
#define PORTConf_PIN_MODE_FLEXRAY                   (Port_PinModeType)5
#define PORTConf_PIN_MODE_ICU                       (Port_PinModeType)6
#define PORTConf_PIN_MODE_LIN                       (Port_PinModeType)7
#define PORTConf_PIN_MODE_MEM                       (Port_PinModeType)8
#define PORTConf_PIN_MODE_PWM                       (Port_PinModeType)9
#define PORTConf_PIN_MODE_SPI                       (Port_PinModeType)10

/* Configuration of symbolic names to be used in Port_PBcfg.c */
/* Port A */
#define PORTConf_PA0_CHANNEL_ID_INDEX        (uint8)0x00
#define PORTConf_PA1_CHANNEL_ID_INDEX        (uint8)0x01
#define PORTConf_PA2_CHANNEL_ID_INDEX        (uint8)0x02
#define PORTConf_PA3_CHANNEL_ID_INDEX        (uint8)0x03
#define PORTConf_PA4_CHANNEL_ID_INDEX        (uint8)0x04
#define PORTConf_PA5_CHANNEL_ID_INDEX        (uint8)0x05
#define PORTConf_PA6_CHANNEL_ID_INDEX        (uint8)0x06
#define PORTConf_PA7_CHANNEL_ID_INDEX        (uint8)0x07
/* Port B */
#define PORTConf_PB0_CHANNEL_ID_INDEX        (uint8)0x08
#define PORTConf_PB1_CHANNEL_ID_INDEX        (uint8)0x09
#define PORTConf_PB2_CHANNEL_ID_INDEX        (uint8)0x0A
#define PORTConf_PB3_CHANNEL_ID_INDEX        (uint8)0x0B
#define PORTConf_PB4_CHANNEL_ID_INDEX        (uint8)0x0C
#define PORTConf_PB5_CHANNEL_ID_INDEX        (uint8)0x0D
#define PORTConf_PB6_CHANNEL_ID_INDEX        (uint8)0x0E
#define PORTConf_PB7_CHANNEL_ID_INDEX        (uint8)0x0F
/* Port C */
#define PORTConf_PC0_CHANNEL_ID_INDEX        (uint8)0x10
#define PORTConf_PC1_CHANNEL_ID_INDEX        (uint8)0x11
#define PORTConf_PC2_CHANNEL_ID_INDEX        (uint8)0x12
#define PORTConf_PC3_CHANNEL_ID_INDEX        (uint8)0x13
#define PORTConf_PC4_CHANNEL_ID_INDEX        (uint8)0x14
#define PORTConf_PC5_CHANNEL_ID_INDEX        (uint8)0x15
#define PORTConf_PC6_CHANNEL_ID_INDEX        (uint8)0x16
#define PORTConf_PC7_CHANNEL_ID_INDEX        (uint8)0x17
/* Port D */
#define PORTConf_PD0_CHANNEL_ID_INDEX        (uint8)0x18
#define PORTConf_PD1_CHANNEL_ID_INDEX        (uint8)0x19
#define PORTConf_PD2_CHANNEL_ID_INDEX        (uint8)0x1A
#define PORTConf_PD3_CHANNEL_ID_INDEX        (uint8)0x1B
#define PORTConf_PD4_CHANNEL_ID_INDEX        (uint8)0x1C
#define PORTConf_PD5_CHANNEL_ID_INDEX        (uint8)0x1D
#define PORTConf_PD6_CHANNEL_ID_INDEX        (uint8)0x1E
#define PORTConf_PD7_CHANNEL_ID_INDEX        (uint8)0x1F
/* Port E */
#define PORTConf_PE0_CHANNEL_ID_INDEX        (uint8)0x20
#define PORTConf_PE1_CHANNEL_ID_INDEX        (uint8)0x21
#define PORTConf_PE2_CHANNEL_ID_INDEX        (uint8)0x22
#define PORTConf_PE3_CHANNEL_ID_INDEX        (uint8)0x23
#define PORTConf_PE4_CHANNEL_ID_INDEX        (uint8)0x24
#define PORTConf_PE5_CHANNEL_ID_INDEX        (uint8)0x25
/* Port F */
#define PORTConf_PF0_CHANNEL_ID_INDEX        (uint8)0x26
#define PORTConf_PF1_CHANNEL_ID_INDEX        (uint8)0x27
#define PORTConf_PF2_CHANNEL_ID_INDEX        (uint8)0x28
#define PORTConf_PF3_CHANNEL_ID_INDEX        (uint8)0x29
#define PORTConf_PF4_CHANNEL_ID_INDEX        (uint8)0x2A



/* Configuration of unused Ports and Port Pins
 * 1. Output Mode
 * 2. No Internal Resistor
 * 3. Initial Value Low
 * 4. DIO Mode
 * 5. Direction is Changeable
 * 6. Mode is Changeable
 */
#define PORTConf_PIN_UNDEFINED_MODE                 PORT_PIN_OUT,\
                                                    OFF,\
                                                    PORT_PIN_LOW,\
                                                    PORTConf_PIN_MODE_DIO,\
                                                    PORT_PIN_DIRECTION_CHANGEABLE,\
                                                    PORT_PIN_MODE_CHANGEABLE

/* Index of Ports to be used in the Structure in Port_PBcfg.c */
#define PORTConf_PORT_ID_0                          (Port_PortType)0
#define PORTConf_PORT_ID_1                          (Port_PortType)1
#define PORTConf_PORT_ID_2                          (Port_PortType)2
#define PORTConf_PORT_ID_3                          (Port_PortType)3
#define PORTConf_PORT_ID_4                          (Port_PortType)4
#define PORTConf_PORT_ID_5                          (Port_PortType)5

/* Index of Pins to be used in the Structure in Port_PBcfg.c */
#define PORTConf_PIN_ID_0                           (Port_PinType)0
#define PORTConf_PIN_ID_1                           (Port_PinType)1
#define PORTConf_PIN_ID_2                           (Port_PinType)2
#define PORTConf_PIN_ID_3                           (Port_PinType)3
#define PORTConf_PIN_ID_4                           (Port_PinType)4
#define PORTConf_PIN_ID_5                           (Port_PinType)5
#define PORTConf_PIN_ID_6                           (Port_PinType)6
#define PORTConf_PIN_ID_7                           (Port_PinType)7




#endif /* PORT_CFG_H */

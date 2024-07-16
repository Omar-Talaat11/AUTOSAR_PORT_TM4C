/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Omar Talaat
 ******************************************************************************/

#include "Port.h"

/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
        ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
        ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
        ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
        ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of PBcfg.c does not match the expected version"
#endif




/* PB structure used with Port_Init API */
const Port_ConfigType Port_Configuration = {
                                            /* Port A */
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_0,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_1,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_2,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_3,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_4,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_5,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_6,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_0,PORTConf_PIN_ID_7,PORTConf_PIN_UNDEFINED_MODE,
                                            /* Port B */
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_0,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_1,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_2,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_3,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_4,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_5,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_6,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_1,PORTConf_PIN_ID_7,PORTConf_PIN_UNDEFINED_MODE,
                                            /* Port C */
                                            /*
                                             * JTAG Pins
                                             * All parameters won't affect except direction changeable and mode changeable
                                             * Those 2 Modes are set to not changeable
                                             *  */
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_0,PORT_PIN_OUT,OFF,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_1,PORT_PIN_OUT,OFF,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_2,PORT_PIN_OUT,OFF,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_3,PORT_PIN_OUT,OFF,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_4,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_5,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_6,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_2,PORTConf_PIN_ID_7,PORTConf_PIN_UNDEFINED_MODE,
                                            /* Port D */
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_0,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_1,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_2,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_3,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_4,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_5,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_6,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_3,PORTConf_PIN_ID_7,PORTConf_PIN_UNDEFINED_MODE,
                                            /* Port E */
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_0,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_1,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_2,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_3,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_4,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_4,PORTConf_PIN_ID_5,PORTConf_PIN_UNDEFINED_MODE,
                                            /* Port F */
                                            PORTConf_PORT_ID_5,PORTConf_PIN_ID_0,PORTConf_PIN_UNDEFINED_MODE,
                                            /* LED PIN Configuration */
                                            PORTConf_PORT_ID_5,PORTConf_PIN_ID_1,PORT_PIN_OUT,OFF,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE,
                                            PORTConf_PORT_ID_5,PORTConf_PIN_ID_2,PORTConf_PIN_UNDEFINED_MODE,
                                            PORTConf_PORT_ID_5,PORTConf_PIN_ID_3,PORTConf_PIN_UNDEFINED_MODE,
                                            /* BUTTON PIN Configuration */
                                            PORTConf_PORT_ID_5,PORTConf_PIN_ID_4,PORT_PIN_IN,PULL_UP,PORT_PIN_LOW,PORTConf_PIN_MODE_DIO,PORT_PIN_DIRECTION_NOT_CHANGEABLE,PORT_PIN_MODE_NOT_CHANGEABLE
};

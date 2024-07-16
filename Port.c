/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Omar Talaat
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
        || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
        || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const PortPin * Port_PortChannels = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
 * Service Name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to post-build configuration data
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the Port module.
 ************************************************************************************/
void Port_Init(const Port_ConfigType*ConfigPtr)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (NULL_PTR == ConfigPtr)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
                        PORT_E_PARAM_CONFIG);
    }
    else
#endif
    {
        uint8 i;
        PortPin * Port_Ptr;
        Port_Ptr = ConfigPtr->PortPins;
        volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
        for( i = 0; i<PORT_NUMBER_OF_PINS;i++)
        {
            PortGpio_Ptr = NULL_PTR;
            switch(Port_Ptr->port_num)
            {
            case  0:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
            case  1:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
            case  2:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
            case  3:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
            case  4:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
            case  5:
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
            }

            if( ((Port_Ptr->port_num == 3) && (Port_Ptr->pin_num == 7)) || ((Port_Ptr->port_num == 5) && (Port_Ptr->pin_num == 0)) ) /* PD7 or PF0 */
            {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = PORT_LOCK_REG_NUM;                     /* Unlock the GPIOCR register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Ptr->pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            else if( (Port_Ptr->port_num == 2) && (Port_Ptr->pin_num <= 3) ) /* PC0 to PC3 */
            {
                /*
                 * Do Nothing ...  this is the JTAG pins
                 * Increment Pointer to Next Pin
                 * Continue to skip this iteration.
                 *  */
                Port_Ptr+=1;
                continue;

            }
            else
            {
                /* Do Nothing ... No need to unlock the commit register for this pin */
            }
            if(Port_Ptr->direction == PORT_PIN_OUT)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Ptr->pin_num);               /* Set the corresponding bit in the GPIODIR register to configure it as output pin */

                if(Port_Ptr->initial_value == PORT_PIN_HIGH)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Ptr->pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }
                else
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_Ptr->pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }
            }
            else if(Port_Ptr->direction == PORT_PIN_IN)
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Ptr->pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */

                if(Port_Ptr->resistor == PULL_UP)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Ptr->pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                }
                else if(Port_Ptr->resistor == PULL_DOWN)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Ptr->pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                }
                else
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_Ptr->pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_Ptr->pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                }
            }
            else
            {
                /* Do Nothing */
            }


            switch(Port_Ptr->mode)
            {
            case 0: /* ADC */
                if( ((Port_Ptr->port_num == 1) && ((Port_Ptr->pin_num == 4)||(Port_Ptr->pin_num == 5))) /* PB4 or PB5 */
                        || ((Port_Ptr->port_num == 3) && ((Port_Ptr->pin_num == 0)||(Port_Ptr->pin_num == 1)||(Port_Ptr->pin_num == 2)||(Port_Ptr->pin_num == 3))) /* PD0 or PD1 or PD2 or PD3 */
                        || ((Port_Ptr->port_num == 4) && ((Port_Ptr->pin_num == 0)||(Port_Ptr->pin_num == 1)||(Port_Ptr->pin_num == 2)||(Port_Ptr->pin_num == 3)||(Port_Ptr->pin_num == 4)||(Port_Ptr->pin_num == 5)))) /* PE0 or PE1 or PE2 or PE3 or PE4 or PE5 */
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);        /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);       /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
                }
                else
                {
                    /* Do Nothing */
                }
                break;

            case 1: /* CAN */
                if( ((Port_Ptr->port_num == 0) && (Port_Ptr->pin_num == 0 )) || ((Port_Ptr->port_num == 1) && (Port_Ptr->pin_num == 4)) || ((Port_Ptr->port_num == 4) && (Port_Ptr->pin_num == 4 )) /* PA0 or PB4 or PE4 */
                        || ((Port_Ptr->port_num == 0) && (Port_Ptr->pin_num == 1 )) || ((Port_Ptr->port_num == 1) && (Port_Ptr->pin_num == 5)) || ((Port_Ptr->port_num == 4) && (Port_Ptr->pin_num == 5 ))) /* PA1 or PB5 or PE5*/
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_Ptr->pin_num * 4));      /* Assign CAN mode to PCTL Register */
                }
                else if ( ((Port_Ptr->port_num == 5) && (Port_Ptr->pin_num == 0 )) || ((Port_Ptr->port_num == 5) && (Port_Ptr->pin_num == 3)) ) /* PF0 or PF3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_Ptr->pin_num * 4));      /* Assign CAN mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;

            case 2: /* DIO */

                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                break;

            case 3: /* GPT */
                if((Port_Ptr->port_num == 0) || (Port_Ptr->port_num == 4)) /* Port A or Port E */
                {
                    /* Do Nothing */
                }
                else
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Port_Ptr->pin_num * 4));      /* Assign Timer mode to PCTL Register */
                }
                break;

            case 4: /* WDG */
                /* Do Nothing */
                break;

            case 5: /* FLEXRAY */
                /* Do Nothing */
                break;

            case 6: /* ICU */
                /* Do Nothing */
                break;

            case 7: /* LIN */
                /* Do Nothing */
                break;

            case 8: /* MEM */
                /* Do Nothing */
                break;

            case 9: /* PWM */
                if( ((Port_Ptr->port_num == 1) && (Port_Ptr->pin_num >3 )) || ((Port_Ptr->port_num == 2) && ((Port_Ptr->pin_num == 4) || (Port_Ptr->pin_num == 5))) || ((Port_Ptr->port_num == 3) && (Port_Ptr->pin_num <2 )) || ((Port_Ptr->port_num == 4) && (Port_Ptr->pin_num >=4)) ) /* PB4:7 or PC4:5 or PD0:1 or PE4:5 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (Port_Ptr->pin_num * 4));      /* Assign PWM mode to PCTL Register */
                }
                else if( ((Port_Ptr->port_num == 0) && (Port_Ptr->pin_num >= 6 )) || ((Port_Ptr->port_num == 5) && ((Port_Ptr->pin_num < 4))) ) /* PA0:6 or PF0:3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (Port_Ptr->pin_num * 4));      /* Assign PWM mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;
            case 10: /* SPI */
                if( ((Port_Ptr->port_num == 0) && ((Port_Ptr->pin_num >= 2 ) && (Port_Ptr->pin_num < 6 ))) || ((Port_Ptr->port_num == 1) && ((Port_Ptr->pin_num >= 4)))|| ((Port_Ptr->port_num == 3) && ((Port_Ptr->pin_num < 4))) || ((Port_Ptr->port_num == 5) && ((Port_Ptr->pin_num < 4))) ) /* PA2:5 or PB4:7 or PD0:3 or PF0:3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_Ptr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_Ptr->pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_Ptr->pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_Ptr->pin_num * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Port_Ptr->pin_num * 4));      /* Assign SPI mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;
            }
            Port_Ptr+=1;
        }
        Port_PortChannels = ConfigPtr->PortPins;
        Port_Status = PORT_INITIALIZED;
    }
}

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number , Port Pin ID number - Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin direction.
 ************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API==STD_ON)
void Port_SetPinDirection(Port_PinType Pin,Port_PinDirectionType Direction)
{
    uint8 PortID;
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the used channel is within the valid range */
    if(PORTConf_PF4_CHANNEL_ID_INDEX<Pin)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                        PORT_E_PARAM_PIN);
        error=TRUE;
    }
    else
    {
        /* No Action Required */
    }
    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                        PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if(FALSE==error)
    {
        if((Port_PortChannels+Pin)->direction_changeable==PORT_PIN_DIRECTION_CHANGEABLE)
        {
            volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

            if(Pin<=PORTConf_PA7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_0;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            }
            else if(Pin<=PORTConf_PB7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_1;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                Pin-=PORTConf_PB0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PC7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_2;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                Pin-=PORTConf_PC0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PD7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_3;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                Pin-=PORTConf_PD0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PE5_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_4;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                Pin-=PORTConf_PE0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PF4_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_5;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                Pin-=PORTConf_PF0_CHANNEL_ID_INDEX;
            }
            else
            {
                /* Do Nothing */
            }
            if(( (PortID == 3) && (Pin == 7)) || ((PortID == 5) && (Pin == 0)) ) /* PD7 or PF0 */
            {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = PORT_LOCK_REG_NUM;                     /* Unlock the GPIOCR register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Pin);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            else if( (PortID == 2) && (Pin <= 3) ) /* PC0 to PC3 */
            {
                /* Do Nothing ...  this is the JTAG pins */
            }
            else
            {
                /* Do Nothing ... No need to unlock the commit register for this pin */
            }

            if(Direction == PORT_PIN_OUT)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Pin);               /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            }
            else if(Direction == PORT_PIN_IN)
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Pin);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            }
            else
            {
                /* Do Nothing */
            }
        }
        else
        {
            /* Do Nothing */
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                            PORT_E_DIRECTION_UNCHANGEABLE);
#endif
        }
    }
    else
    {

        /* Do Nothing */
    }

}
#endif

/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes port direction.
 ************************************************************************************/
void Port_RefreshPortDirection(void)
{
    uint8 i;
    PortPin * Port_Ptr;
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)

    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID,
                        PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if(FALSE==error){
        for( i = 0; i<PORT_NUMBER_OF_PINS;i++)
        {
            Port_Ptr=(Port_PortChannels+i);
            if(Port_Ptr->direction_changeable==PORT_PIN_DIRECTION_NOT_CHANGEABLE)
            {
                PortGpio_Ptr = NULL_PTR;
                switch(Port_Ptr->port_num)
                {
                case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
                case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
                case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
                case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
                case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
                case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
                }
                if( ((Port_Ptr->port_num == 3) && (Port_Ptr->pin_num == 7)) || ((Port_Ptr->port_num == 5) && (Port_Ptr->pin_num == 0)) ) /* PD7 or PF0 */
                {
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = PORT_LOCK_REG_NUM;                     /* Unlock the GPIOCR register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_Ptr->pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
                }
                else if( (Port_Ptr->port_num == 2) && (Port_Ptr->pin_num <= 3) ) /* PC0 to PC3 */
                {
                    continue;
                    /* Do Nothing ...  this is the JTAG pins */
                }
                else
                {
                    /* Do Nothing ... No need to unlock the commit register for this pin */
                }
                if(Port_Ptr->direction == PORT_PIN_OUT)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Ptr->pin_num);               /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                }
                else if(Port_Ptr->direction == PORT_PIN_IN)
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_Ptr->pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                }
                else
                {
                    /* Do Nothing */
                }
            }
            else
            {
                /* Do Nothing */
            }
        }
    }
    else
    {
        /* Do Nothing */
    }

}


/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Returns the version information of this module.
 ************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if input pointer is not Null pointer */
    if(NULL_PTR == versioninfo)
    {
        /* Report to DET  */
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                        PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID,
                        PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if(FALSE==error){
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
    else
    {
        /* Do Nothing */
    }
}
#endif


/************************************************************************************
 * Service Name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number , Mode - New Port Pin mode to be set on port pin.
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Returns the version information of this module.
 ************************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
    boolean error = FALSE;
    uint8 PortID;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* Check if the used channel is within the valid range */
    if(PORTConf_PF4_CHANNEL_ID_INDEX<Pin)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                        PORT_E_PARAM_PIN);
        error=TRUE;
    }
    else
    {
        /* No Action Required */
    }
    /* Check if the Driver is initialized before using this function */
    if (PORT_NOT_INITIALIZED == Port_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                        PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* No Action Required */
    }
    /* Check if the used mode is within the valid range */
    if(PORTConf_PIN_MODE_SPI<Mode)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                        PORT_E_PARAM_INVALID_MODE);
        error=TRUE;
    }
    else
    {
        /* No Action Required */
    }
#endif
    if(FALSE==error)
    {
        if((Port_PortChannels+Pin)->mode_changeable==PORT_PIN_MODE_CHANGEABLE)
        {
            volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

            if(Pin<=PORTConf_PA7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_0;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            }
            else if(Pin<=PORTConf_PB7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_1;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                Pin-=PORTConf_PB0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PC7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_2;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                Pin-=PORTConf_PC0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PD7_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_3;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                Pin-=PORTConf_PD0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PE5_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_4;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                Pin-=PORTConf_PE0_CHANNEL_ID_INDEX;
            }
            else if(Pin<=PORTConf_PF4_CHANNEL_ID_INDEX)
            {
                PortID = PORTConf_PORT_ID_5;
                PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                Pin-=PORTConf_PF0_CHANNEL_ID_INDEX;
            }
            else
            {
                /* Do Nothing */
            }

            if(( (PortID == 3) && (Pin == 7)) || ((PortID == 5) && (Pin == 0)) ) /* PD7 or PF0 */
            {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = PORT_LOCK_REG_NUM;                     /* Unlock the GPIOCR register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Pin);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            else if( (PortID == 2) && (Pin <= 3) ) /* PC0 to PC3 */
            {
                /* Do Nothing ...  this is the JTAG pins */
            }
            else
            {
                /* Do Nothing ... No need to unlock the commit register for this pin */
            }

            switch(Mode)
            {
            case 0: /* ADC */
                if( ((PortID == 1) && ((Pin == 4)||(Pin == 5))) /* PB4 or PB5 */
                        || ((PortID == 3) && ((Pin == 0)||(Pin == 1)||(Pin == 2)||(Pin == 3))) /* PD0 or PD1 or PD2 or PD3 */
                        || ((PortID == 4) && ((Pin == 0)||(Pin == 1)||(Pin == 2)||(Pin == 3)||(Pin == 4)||(Pin == 5)))) /* PE0 or PE1 or PE2 or PE3 or PE4 or PE5 */
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);        /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);       /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
                }
                else
                {
                    /* Do Nothing */
                }
                break;

            case 1: /* CAN */
                if( ((PortID == 0) && (Pin == 0 )) || ((PortID == 1) && (Pin == 4)) || ((PortID == 4) && (Pin == 4 )) /* PA0 or PB4 or PE4 */
                        || ((PortID == 0) && (Pin == 1 )) || ((PortID == 1) && (Pin == 5)) || ((PortID == 4) && (Pin == 5 ))) /* PA1 or PB5 or PE5*/
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Pin * 4));      /* Assign CAN mode to PCTL Register */

                }
                else if ( ((PortID == 5) && (Pin == 0 )) || ((PortID == 5) && (Pin == 3)) ) /* PF0 or PF3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Pin * 4));      /* Assign CAN mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;

            case 2: /* DIO */

                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                break;

            case 3: /* GPT */
                if((PortID == 0) || (PortID == 4)) /* Port A or Port E */
                {
                    /* Do Nothing */
                }
                else
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Pin * 4));      /* Assign Timer mode to PCTL Register */
                }
                break;

            case 4: /* WDG */
                /* Do Nothing */
                break;

            case 5: /* FLEXRAY */
                /* Do Nothing */
                break;

            case 6: /* ICU */
                /* Do Nothing */
                break;

            case 7: /* LIN */
                /* Do Nothing */
                break;

            case 8: /* MEM */
                /* Do Nothing */
                break;

            case 9: /* PWM */
                if( ((PortID == 1) && (Pin >3 )) || ((PortID == 2) && ((Pin == 4) || (Pin == 5))) || ((PortID == 3) && (Pin <2 )) || ((PortID == 4) && (Pin >=4)) ) /* PB4:7 or PC4:5 or PD0:1 or PE4:5 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (Pin * 4));      /* Assign PWM mode to PCTL Register */
                }
                else if( ((PortID == 0) && (Pin >= 6 )) || ((PortID == 5) && ((Pin < 4))) ) /* PA0:6 or PF0:3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (Pin * 4));      /* Assign PWM mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;
            case 10: /* SPI */
                if( ((PortID == 0) && ((Pin >= 2 ) && (Pin < 6 ))) || ((PortID == 1) && ((Pin >= 4)))|| ((PortID == 3) && ((Pin < 4))) || ((PortID == 5) && ((Pin < 4))) ) /* PA2:5 or PB4:7 or PD0:3 or PF0:3 */
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Pin);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));     /* Clear the PMCx bits for this pin */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Pin * 4));      /* Assign SPI mode to PCTL Register */
                }
                else
                {
                    /* Do Nothing */
                }
                break;
            }
        }
        else
        {
#if (PORT_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                            PORT_E_MODE_UNCHANGEABLE);
#endif
            /* Do Nothing */
        }

    }
    else
    {
        /* Do Nothing */
    }
}
#endif

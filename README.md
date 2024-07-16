AUTOSAR port driver for the TM4C Microcontroller!

**Following the AUTOSAR SWS document for the port driver, I structured the driver files as follows:**
    Port_Regs.h: Contains addresses of the needed registers.
    Port_Cfg.h: Holds all pre-compile configurations.
    Port_PBcfg.c: Manages post-build configurations.
    Port.h: Includes data types, function prototypes, and various IDs.
    Port.c: Contains the source code of the functions.

**The driver features 5 key APIs:**
    Port_Init: Initializes all the microcontroller pins.
    Port_SetPinDirection: Changes the direction of a specific pin.
    Port_RefreshPortDirection: Refreshes the directions of all pins.
    Port_GetVersionInfo: Identifies the version of the driver.
    Port_SetPinMode: Changes the mode of a specific pin.

Additionally, the driver includes all DET errors, facilitating error detection during development.

**You can find also 2 additional files:**
    1-Common_Macros.h: for some commonly used macros in the driver
    2-Std_Types.h: for the standard AUTOSAR types

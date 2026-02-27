/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "matrix.h"
#include "numeric.h"
#include "link.h"
#include "led.h"
#include "lcd.h"
#include "anim.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    MTX_Init();
    CORETIMER_DelayMs(250);
    
    MTX_WriteText(DISP_BEAMMODE, "LINK...");
    LINK_Init();
    CORETIMER_DelayMs(150);
    
    MTX_WriteClearText(DISP_BEAMMODE, "LCD...");
    LCD_Init();
    CORETIMER_DelayMs(500);
    
    MTX_WriteText(DISP_BEAMMODE, "NUM...");
    NUM_Init();
    CORETIMER_DelayMs(250);
    
    MTX_WriteText(DISP_BEAMMODE, "LED...");
    LED_Init();
    CORETIMER_DelayMs(250);
    
    MTX_WriteText(DISP_BEAMMODE, "ANIM...");
    ANIM_Init();
    CORETIMER_DelayMs(150);
    
    MTX_WriteClearText(DISP_ACCELMODE, "LHC");
    MTX_WriteClearText(DISP_BEAMMODE, "RUN");

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        LINK_ProcessCommand();
        
        // TODO: I'd rather put this on a ~10ms timer than call it this frequently
        ANIM_Loop();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/


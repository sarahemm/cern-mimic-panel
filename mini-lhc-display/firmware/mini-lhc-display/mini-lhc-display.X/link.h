/* 
 * File:   link.h
 * Author: Sen
 *
 * Created on October 4, 2025, 6:19 PM
 */

#ifndef LINK_H
#define	LINK_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"                // SYS function prototypes

    void LINK_Init(void);
    bool LINK_Callback(I2C_SLAVE_TRANSFER_EVENT event, uintptr_t contextHandle);
    void LINK_ProcessCommand(void);
    bool LINK_IsDebugFlagSet(void);
    
#define LINK_ID_ENERGY        0x0000
#define LINK_ID_TSB           0x0001
#define LINK_ID_ACCELMODE     0x0010
#define LINK_ID_BEAMMODE      0x0011
#define LINK_ID_COMMENT       0x0012

// Wrapup LEDs
#define LINK_ID_CRYO_WRAPUP   0x0020
#define LINK_ID_COLLIMATOR_B1 0x0021
#define LINK_ID_COLLIMATOR_B2 0x0022

// BIS/SMP LEDs
#define LINK_ID_BEAMPERMIT_B1 0x0030
#define LINK_ID_STABLEBEAM_B1 0x0031
#define LINK_ID_MOVABLES_B1   0x0032
#define LINK_ID_BEAMPERMIT_B2 0x0033
#define LINK_ID_STABLEBEAM_B2 0x0034
#define LINK_ID_MOVABLES_B2   0x0035
#define LINK_ID_PERMITSLINKED 0x0036

// Injection Tunnel LEDs
#define LINK_ID_TI2_FIRST     0x0040  // 3 total
#define LINK_ID_TI8_FIRST     0x0043  // 3 total

// Dump LEDs
#define LINK_ID_B1_DUMP_FIRST 0x0050  // 2 total
#define LINK_ID_B1_DUMP       0x0052
#define LINK_ID_B2_DUMP_FIRST 0x0053  // 2 total
#define LINK_ID_B2_DUMP       0x0055

// IP LEDs
#define LINK_ID_IP1           0x0060
#define LINK_ID_IP2           0x0061
#define LINK_ID_IP5           0x0062
#define LINK_ID_IP8           0x0063

// Beam LEDs
#define LINK_ID_B1_FIRST      0x0070  // 24 total, 0x0070 - 0x0088
#define LINK_ID_B2_FIRST      0x0090  // 24 total, 0x0090 - 0x00A8

#define LINK_FLAG_DEBUG              0x01

#define LINK_CMD_NOOP                0x00
#define LINK_CMD_SET_SYSFLAG         0x01
#define LINK_CMD_CLEAR_SYSFLAG       0x02
    
#define LINK_CMD_SET_LED             0x10
#define LINK_CMD_SET_LED_2CH         0x11
#define LINK_CMD_SET_LED_3CH         0x12
#define LINK_CMD_SET_LED_MULTI       0x13
#define LINK_CMD_SET_LED_2CH_MULTI   0x14
#define LINK_CMD_SET_LED_3CH_MULTI   0x15

#define LINK_CMD_SET_NUMERIC         0x20
#define LINK_CMD_SET_TEXT            0x21
#define LINK_CMD_INIT_TEXT_BUFFER    0x22
#define LINK_CMD_FILL_TEXT_BUFFER    0x23
#define LINK_CMD_SET_TEXT_FROM_BUFFER 0x24
        

#define LINK_CMD_START_ANIMATION     0x30
#define LINK_CMD_STOP_ANIMATION      0x31
#define LINK_CMD_ANIMATION_MASK_APPLY   0x32
#define LINK_CMD_ANIMATION_MASK_REMOVE  0x33

#ifdef	__cplusplus
}
#endif

#endif	/* LINK_H */


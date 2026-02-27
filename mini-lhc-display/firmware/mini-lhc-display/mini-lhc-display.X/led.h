/* 
 * File:   led.h
 * Author: Sen
 *
 * Created on October 10, 2025, 3:56 PM
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"
#include <string.h> // for memset

    struct fc_data {
        bool DSPRPT;
        bool TMGRST;
        bool RFRESH;
        bool ESPWM;
        bool LSDVLT;
    };
    
    struct tlc5955 {
        uint16_t gs[48];        // 768-bit greyscale latch
        uint8_t dc[42];         // 336-bit dot correction latch
        uint8_t mc[2];          // 9-bit maximum current data (NOT A MULTIPLE OF 8)
        uint8_t bc[4];          // 21-bit global brightness control data (NOT A MULTIPLE OF 8)
        struct fc_data fc;      // 5-bit function control data
    };

    
    void LED_Init(void);
    void LED_InitDriver(struct tlc5955 *);
    void LED_StartupDance(void);
    void LED_WriteGS(void);
    void LED_WriteSingleGS(struct tlc5955);
    void LED_WriteControl(void);
    void LED_WriteSingleControl(struct tlc5955);
    void LED_Toggle_Clk(void);
    void LED_Toggle_Lat(void);
    void LED_Set_SDO(bool);

    void LED_Clear_Beam(uint8_t);
    void LED_Set_Beam(uint8_t, uint8_t, uint16_t);
    
    void LED_Clear_Misc(void);
    void LED_Set_Misc(uint8_t, uint16_t);
    void LED_Set(uint8_t, uint8_t, uint16_t);
    void LED_Clear_Drvr(uint8_t);

#define LED_DRVR_MISC       0
#define LED_DRVR_BEAM_1     1
#define LED_DRVR_BEAM_2     2
#define LED_DRVR_BOTH_BEAMS 3   // used by the animation routines, not a real drvr but we expand it out to be both beams 1 and 2 when used

#define LED_IP1_G           0
#define LED_IP1_R           1
#define LED_IP1_B           2
#define LED_DUMP_B2_G       3
#define LED_DUMP_B2_R       4
#define LED_DUMP_B2_B       5
#define LED_IP5_G           6
#define LED_IP5_R           7
#define LED_IP5_B           8
#define LED_DUMP_B1_G       9
#define LED_DUMP_B1_R       10
#define LED_DUMP_B1_B       11
#define LED_IP2_G           12
#define LED_IP2_R           13
#define LED_IP2_B           14
#define LED_IP8_G           15
#define LED_IP8_R           16
#define LED_IP8_B           17
#define LED_DUMP_TUN_B1_1   18
#define LED_DUMP_TUN_B1_2   19
#define LED_DUMP_TUN_B2_2   20
#define LED_DUMP_TUN_B2_1   21
#define LED_TI8_1           22
#define LED_TI8_2           23
#define LED_TI2_3           24
#define LED_TI2_2           25
#define LED_TI8_3           26
#define LED_B2_COLL_R       27
#define LED_B1_COLL_R       28
#define LED_B2_COLL_G       29
#define LED_PERMITLINK_G    30
#define LED_PERMITLINK_R    31
#define LED_B1_MOVABLES_R   32
#define LED_B2_PERMIT_R     33
#define LED_B2_PERMIT_G     34
#define LED_B2_STABLE_R     35
#define LED_CRYOSTATUS_G    36
#define LED_CRYOSTATUS_R    37
#define LED_TI2_1           38
#define LED_B1_PERMIT_G     39
#define LED_B1_PERMIT_R     40
#define LED_B1_COLL_G       41
#define LED_B1_STABLE_R     42
#define LED_B1_MOVABLES_G   43
#define LED_B1_STABLE_G     44
#define LED_B2_MOVABLES_R   45
#define LED_B2_STABLE_G     46
#define LED_B2_MOVABLES_G   47    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */


/* 
 * File:   anim.h
 * Author: Sen
 *
 * Created on October 26, 2025, 7:57 PM
 */

#ifndef ANIM_H
#define	ANIM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"
    struct anim_action_led {
        uint8_t led_drvr;
        uint8_t led;
    };
    
    struct anim_action_delay {
        uint16_t delay_ms;
    };
    
    struct anim_action_goto {
        uint8_t goto_step;
        uint8_t unused;
    };

    struct anim_frame {
        uint8_t anim_action;
        union {
            struct anim_action_led led_action;
            struct anim_action_delay delay_action;
            struct anim_action_goto goto_action;
        };
    };

    void ANIM_Init(void);
    void ANIM_StartAnimation(uint8_t);
    void ANIM_StopAnimation(void);
    void ANIM_Loop(void);
    void ANIM_Mask_Apply(uint8_t);
    void ANIM_Mask_Remove(uint8_t);
    void ANIM_LED_Set(uint8_t, uint8_t, uint16_t);


#define ANIM_GetTimerEndTime(ms) ((CORE_TIMER_FREQUENCY / 1000U) * ms) + CORETIMER_CounterGet();

#define ANIM_NONE           0
#define ANIM_INJECTING      1
#define ANIM_INJECTING2     5
#define ANIM_DUMPING        2
#define ANIM_BOTH_BEAMS     3
#define ANIM_BOTH_BEAMS_EXP 4

#define ANIM_ACTION_LED         0   // clear the driver, then set the specified LED on
#define ANIM_ACTION_LED_ADD     1   // set the specified LED on without clearing the driver first
#define ANIM_ACTION_LED_CLEAR   2   // clear all LEDs on the specified driver
#define ANIM_ACTION_DELAY       3   // delay the given number of milliseconds
#define ANIM_ACTION_GOTO        4   // goto a specific frame (usually 0 as the last step))

#define ANIM_MASK_BEAM_1        0x01    // hide beam 1
#define ANIM_MASK_BEAM_2        0x02    // hide beam 2
#define ANIM_MASK_IP_1          0x04    // hide interactions at ATLAS
#define ANIM_MASK_IP_2          0x08    // hide interactions at ALICE
#define ANIM_MASK_IP_5          0x10    // hide interactions at CMS
#define ANIM_MASK_IP_8          0x20    // hide interactions at LHCb


#ifdef	__cplusplus
}
#endif

#endif	/* ANIM_H */


#include "anim.h"
#include "led.h"

// temporary, for debugging
#include "lcd.h"
#include "stdio.h"

uint8_t anim_current;
uint8_t anim_frame_nbr;
uint32_t anim_waiting_until;
uint8_t anim_mask;

#include "anim-animdata.h"

struct anim_frame *anim_data[16];

void ANIM_Init(void) {
    anim_mask = 0;
    anim_data[ANIM_BOTH_BEAMS] = anim_stable;
    //anim_data[ANIM_INJECTING] = anim_injecting;
    anim_data[ANIM_INJECTING2] = anim_injecting2;
    anim_data[ANIM_BOTH_BEAMS_EXP] = anim_stable_with_exp;
    
    anim_current = ANIM_NONE;
}

void ANIM_StartAnimation(uint8_t anim) {
    anim_current = anim;
    anim_frame_nbr = 0;
    
    LED_Clear_Drvr(LED_DRVR_MISC);
    LED_Clear_Drvr(LED_DRVR_BEAM_1);
    LED_Clear_Drvr(LED_DRVR_BEAM_2);
}

void ANIM_StopAnimation(void) {
    anim_current = ANIM_NONE;
    anim_frame_nbr = 0;
    
    LED_Clear_Drvr(LED_DRVR_MISC);
    LED_Clear_Drvr(LED_DRVR_BEAM_1);
    LED_Clear_Drvr(LED_DRVR_BEAM_2);
}

// process whatever animation steps are next
// we move through any number of LED actions until we hit a delay action, then stop and return
void ANIM_Loop(void) {
    if(anim_current == ANIM_NONE) return;
    
    // if we're waiting until a specific time, return immediately unless that time has arrived
    if(anim_waiting_until > 0 && CORETIMER_CounterGet() < anim_waiting_until) return;
    if(anim_waiting_until != 0) anim_waiting_until = 0;
    
    char buf[19];
    sprintf(buf, "anim: c%d f%d  ", anim_current, anim_frame_nbr);
    LCD_Write_Debug_Text(6, buf);

    while(true) {
        switch(anim_data[anim_current][anim_frame_nbr].anim_action) {
            case ANIM_ACTION_LED:
                LED_Clear_Drvr(anim_data[anim_current][anim_frame_nbr].led_action.led_drvr);
                ANIM_LED_Set(anim_data[anim_current][anim_frame_nbr].led_action.led_drvr, anim_data[anim_current][anim_frame_nbr].led_action.led, 65000);
                anim_frame_nbr++;
                break;
            case ANIM_ACTION_LED_ADD:
                ANIM_LED_Set(anim_data[anim_current][anim_frame_nbr].led_action.led_drvr, anim_data[anim_current][anim_frame_nbr].led_action.led, 65000);
                anim_frame_nbr++;
                break;
            case ANIM_ACTION_LED_CLEAR:
                LED_Clear_Drvr(anim_data[anim_current][anim_frame_nbr].led_action.led_drvr);
                anim_frame_nbr++;
                break;
            case ANIM_ACTION_DELAY:
                LED_WriteGS();
                anim_waiting_until = ANIM_GetTimerEndTime(anim_data[anim_current][anim_frame_nbr].delay_action.delay_ms);
                anim_frame_nbr++;
                return;
            case ANIM_ACTION_GOTO:
                anim_frame_nbr = anim_data[anim_current][anim_frame_nbr].goto_action.goto_step;
                break;
        }
    }
}

void ANIM_Mask_Apply(uint8_t mask) {
    anim_mask |= mask;
}

void ANIM_Mask_Remove(uint8_t mask) {
    anim_mask &= ~mask;
}

// apply any required masks, then pass requests on to LED_Set to actually set the LEDs
void ANIM_LED_Set(uint8_t driver, uint8_t led, uint16_t value) {
    uint8_t masked_drvr = driver;
    
    switch(driver) {
        case LED_DRVR_BEAM_1:
            if(anim_mask & ANIM_MASK_BEAM_1) return;
            break;
        case LED_DRVR_BEAM_2:
            if(anim_mask & ANIM_MASK_BEAM_2) return;
            break;
        case LED_DRVR_BOTH_BEAMS:
            if(anim_mask & ANIM_MASK_BEAM_1) masked_drvr = LED_DRVR_BEAM_2;
            if(anim_mask & ANIM_MASK_BEAM_2) masked_drvr = LED_DRVR_BEAM_1;
            break;
        case LED_DRVR_MISC:
            // IPs which can be masked off
            if((anim_mask & ANIM_MASK_IP_1) && (led == LED_IP1_R || led == LED_IP1_G || led == LED_IP1_B)) return;
            if((anim_mask & ANIM_MASK_IP_2) && (led == LED_IP2_R || led == LED_IP2_G || led == LED_IP2_B)) return;
            if((anim_mask & ANIM_MASK_IP_5) && (led == LED_IP5_R || led == LED_IP5_G || led == LED_IP5_B)) return;
            if((anim_mask & ANIM_MASK_IP_8) && (led == LED_IP8_R || led == LED_IP8_G || led == LED_IP8_B)) return;
            // injection tunnels need the beam masks applied
            if((anim_mask & ANIM_MASK_BEAM_1) && (led == LED_TI2_1 || led == LED_TI2_2 || led == LED_TI2_3)) return;
            if((anim_mask & ANIM_MASK_BEAM_2) && (led == LED_TI8_1 || led == LED_TI8_2 || led == LED_TI8_3)) return;
            // dump system also needs the beam masks applied
            if((anim_mask & ANIM_MASK_BEAM_1) && (led == LED_DUMP_TUN_B1_1 || led == LED_DUMP_TUN_B1_2)) return;
            if((anim_mask & ANIM_MASK_BEAM_2) && (led == LED_DUMP_TUN_B2_1 || led == LED_DUMP_TUN_B2_2)) return;
            if((anim_mask & ANIM_MASK_BEAM_1) && (led == LED_DUMP_B1_R || led == LED_DUMP_B1_G || led == LED_DUMP_B1_B)) return;
            if((anim_mask & ANIM_MASK_BEAM_2) && (led == LED_DUMP_B2_R || led == LED_DUMP_B2_G || led == LED_DUMP_B2_B)) return;
            break;
    }
    
    LED_Set(masked_drvr, led, value);
}

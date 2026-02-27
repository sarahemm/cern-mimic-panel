#include "link.h"
#include "matrix.h"
#include "led.h"
#include <stdio.h>
#include "lcd.h"
#include "numeric.h"
#include "anim.h"

uint8_t link_buffer[128];
uint8_t link_buflen;
uint8_t link_flags;

char *text_buffer;

void LINK_Init(void) {
    link_buflen = 0;
    link_flags = 0;
    I2C2_CallbackRegister(LINK_Callback, 1);
    
    text_buffer = NULL;
}

void LINK_ReceiveByte(uint8_t byte) {
    link_buffer[link_buflen] = byte;
    link_buflen++;
}

void LINK_Cmd_SetText(uint16_t id, char *buf) {
    switch(id) {
        case LINK_ID_ACCELMODE:
            MTX_WriteClearText(DISP_ACCELMODE, buf);
            break;
        case LINK_ID_BEAMMODE:
            MTX_WriteClearText(DISP_BEAMMODE, buf);
            break;
        case LINK_ID_COMMENT:
            LCD_Clear();
            LCD_Write_Multiline_Text(buf);
            break;
    }
}

void LINK_Cmd_SetNumeric(uint16_t id, uint16_t value) {
    char buf[8];
    switch(id) {
        case LINK_ID_ENERGY:
            sprintf(buf, "%4d", value);
            NUM_WriteDigit(7, buf[0]);
            NUM_WriteDigit(6, buf[1]);
            NUM_WriteDigit(5, buf[2]);
            NUM_WriteDigit(4, buf[3]);
            break;
        case LINK_ID_TSB:
            sprintf(buf, "%04d", value);
            if(value == 0) {
                // we're displaying a value of just zero, so blank the display instead
                NUM_WriteDigit(0, ' ');
                NUM_WriteDigit(1, ' ');
                NUM_WriteDigit(2, ' ');
                NUM_WriteDigit(3, ' ');
            } else {
                NUM_WriteDigit(0, buf[0]);
                NUM_WriteDigit(1, buf[1] | 0x80);   // show the colon, too
                NUM_WriteDigit(2, buf[2]);
                NUM_WriteDigit(3, buf[3]);
            }
            break;
    }
}

void Link_Cmd_SetMultiLED(uint16_t id, uint8_t nbr_leds, uint8_t *buf) {
    uint8_t i;
    uint8_t beam;
    if(id == LINK_ID_B1_FIRST) {
        beam = 0;
    } else {
        beam = 1;
    }
    for(i=0; i<nbr_leds; i++) {
        LED_Set_Beam(beam, i, buf[i] << 8);
    }
    LED_WriteGS();
}

void LINK_CMD_SetLED2Ch(uint16_t id, uint8_t chan1, uint8_t chan2) {
    switch(id) {
        case LINK_ID_BEAMPERMIT_B1:
            LED_Set_Misc(LED_B1_PERMIT_R, chan1<<8);
            LED_Set_Misc(LED_B1_PERMIT_G, chan2<<8);
            break;
        case LINK_ID_BEAMPERMIT_B2:
            LED_Set_Misc(LED_B2_PERMIT_R, chan1<<8);
            LED_Set_Misc(LED_B2_PERMIT_G, chan2<<8);
            break;
        case LINK_ID_STABLEBEAM_B1:
            LED_Set_Misc(LED_B1_STABLE_R, chan1<<8);
            LED_Set_Misc(LED_B1_STABLE_G, chan2<<8);
            break;
        case LINK_ID_STABLEBEAM_B2:
            LED_Set_Misc(LED_B2_STABLE_R, chan1<<8);
            LED_Set_Misc(LED_B2_STABLE_G, chan2<<8);
            break;
        case LINK_ID_MOVABLES_B1:
            LED_Set_Misc(LED_B1_MOVABLES_R, chan1<<8);
            LED_Set_Misc(LED_B1_MOVABLES_G, chan2<<8);
            break;
        case LINK_ID_MOVABLES_B2:
            LED_Set_Misc(LED_B2_MOVABLES_R, chan1<<8);
            LED_Set_Misc(LED_B2_MOVABLES_G, chan2<<8);
            break;
        case LINK_ID_PERMITSLINKED:
            LED_Set_Misc(LED_PERMITLINK_R, chan1<<8);
            LED_Set_Misc(LED_PERMITLINK_G, chan2<<8);
            break;
        case LINK_ID_CRYO_WRAPUP:
            LED_Set_Misc(LED_CRYOSTATUS_R, chan1<<8);
            LED_Set_Misc(LED_CRYOSTATUS_G, chan2<<8);
            break;
    }
    LED_WriteGS();
}

void LINK_Cmd_InitTextBuffer() {
    if(text_buffer != NULL) {
        free(text_buffer);
        text_buffer = NULL;
    }
    text_buffer = malloc(1);    // start out allocating just enough for a terminating null
    *text_buffer = 0;
}

void LINK_Cmd_FillTextBuffer(uint16_t id, char *buf) {
    char *new_ptr;
    new_ptr = realloc(text_buffer, strlen(text_buffer) + strlen(buf) + 1);
    if(new_ptr == NULL) {
        // we were denied any more memory, not much we can do so leave things as-is
        // TODO: should flag this as an error somewhere
        return;
    }
    text_buffer = new_ptr;
    strcat(text_buffer, buf);
}

void LINK_Cmd_SetTextFromBuffer(uint16_t id) {
    switch(id) {
        case LINK_ID_COMMENT:
            LCD_Clear();
            LCD_Write_Multiline_Text(text_buffer);
            break;
    }
    LINK_Cmd_InitTextBuffer();
}
// interrupt handler bottom-half (run in the main event loop)
// process a command if we have a full one, and clear the buffer afterwards.
// if we don't have a full command yet, then do nothing.
void LINK_ProcessCommand(void) {
    char buf[32];
    sprintf(buf, "buflen: %04d", link_buflen);
    LCD_Write_Debug_Text(2, buf);
    if(link_buflen < 3) return; // we can't do anything without an ID and a verb
    
    uint16_t id = ((uint16_t)link_buffer[0] << 8) | link_buffer[1];
    uint8_t verb = link_buffer[2];
    sprintf(buf, "verb: %d  ", verb);
    LCD_Write_Debug_Text(1, buf);
    
    uint8_t consumed_bytes = 0;  // total bytes used by the command, if one was processed
    switch(verb) {
        case LINK_CMD_SET_LED_2CH:
            if(link_buflen < 5) return; // we need at least an id, verb, and two one-byte channel values
            LINK_CMD_SetLED2Ch(id, link_buffer[3], link_buffer[4]);
            consumed_bytes = 5;
            break;
        case LINK_CMD_SET_TEXT:
            if(memchr((void *)&link_buffer[3], 0, link_buflen-3) == NULL) return;   // we need a null-terminated string to continue
            consumed_bytes = memchr((void *)&link_buffer[3], 0, link_buflen-3) - (void *)&link_buffer + 1;
            LINK_Cmd_SetText(id, (char *)&link_buffer[3]);
            break;
        case LINK_CMD_INIT_TEXT_BUFFER:
            // we don't need any parameters, if we got here we have everything we need
            consumed_bytes = 3;
            LINK_Cmd_InitTextBuffer();
            break;
        case LINK_CMD_FILL_TEXT_BUFFER:
            if(memchr((void *)&link_buffer[3], 0, link_buflen-3) == NULL) return;   // we need a null-terminated string to continue
            consumed_bytes = memchr((void *)&link_buffer[3], 0, link_buflen-3) - (void *)&link_buffer + 1;
            LINK_Cmd_FillTextBuffer(id, (char *)&link_buffer[3]);
            break;
        case LINK_CMD_SET_TEXT_FROM_BUFFER:
            // we don't need any parameters, if we got here we have everything we need
            consumed_bytes = 3;
            LINK_Cmd_SetTextFromBuffer(id);
            break;            
        case LINK_CMD_SET_NUMERIC:
            if(link_buflen < 5) return; // we need at least an id, verb, and two-byte value to continue
            uint16_t value = ((uint16_t)link_buffer[3] << 8) | link_buffer[4];
            LINK_Cmd_SetNumeric(id, value);
            consumed_bytes = 5;
            break;
        case LINK_CMD_SET_LED_MULTI:
            if(link_buflen < 4) return; // we need at least an id, verb, and length to continue
            if(link_buflen < 4 + link_buffer[3]) return;
            consumed_bytes = 4 + link_buffer[3];
            Link_Cmd_SetMultiLED(id, link_buffer[3], &link_buffer[4]);
            break;
        case LINK_CMD_START_ANIMATION:
            if(link_buflen < 4) return; // we need at least an id, verb, and one byte anim_id to continue
            ANIM_StartAnimation(link_buffer[3]);
            consumed_bytes = 4;
            break;
        case LINK_CMD_STOP_ANIMATION:
            // we don't need any parameters, if we got here we have everything we need
            consumed_bytes = 3;
            ANIM_StopAnimation();
            break;
        case LINK_CMD_ANIMATION_MASK_APPLY:
            if(link_buflen < 4) return; // we need at least an id, verb, and a one byte mask to continue
            consumed_bytes = 4;
            ANIM_Mask_Apply(link_buffer[3]);
            break;
        case LINK_CMD_ANIMATION_MASK_REMOVE:
            if(link_buflen < 4) return; // we need at least an id, verb, and a one byte mask to continue
            consumed_bytes = 4;
            ANIM_Mask_Remove(link_buffer[3]);
            break;
        case LINK_CMD_SET_SYSFLAG:
            if(link_buflen < 4) return; // we need at least an id, verb, and one byte flag to continue
            link_flags |= link_buffer[3];
            consumed_bytes = 4;
            break;
        case LINK_CMD_CLEAR_SYSFLAG:
            if(link_buflen < 4) return; // we need at least an id, verb, and one byte flag to continue
            link_flags &= ~link_buffer[3];
            consumed_bytes = 4;
            break;
    }
    
    if(consumed_bytes == 0) return;
    // slide the contents of the buffer along to eliminate anything we consumed
    memmove(&link_buffer, &link_buffer[consumed_bytes], sizeof(link_buffer)-consumed_bytes);
    link_buflen -= consumed_bytes;
}

// interrupt handler top-half
bool LINK_Callback(I2C_SLAVE_TRANSFER_EVENT event, uintptr_t contextHandle) {
    //if(event != I2C_SLAVE_TRANSFER_EVENT_RX_READY) return true;
    if(event == I2C_SLAVE_TRANSFER_EVENT_ADDR_MATCH) {
        // TODO: figure out how to re-sync if we get out of sync, since right now we'll probably hang forever and overflow the buffer
        // we can't just clear the buffer anymore, since we may be receiving the next command before the previous is ProcessCommand'ed
        //link_buflen = 0;
    } else if(event == I2C_SLAVE_TRANSFER_EVENT_RX_READY) {
        LINK_ReceiveByte(I2C2_ReadByte());
        //LINK_ProcessCommand();
    }
    
    return true;
}

bool LINK_IsDebugFlagSet(void) {
    return (link_flags & LINK_FLAG_DEBUG) != 0;
}
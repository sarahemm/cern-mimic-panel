#include <string.h>
#include "matrix.h"

void MTX_Init(void) {
    MTX_Takeover();
    MTX_Reset();
    
    MTX_WriteText(DISP_ACCELMODE, "INIT...");
    MTX_WriteText(DISP_BEAMMODE, "MTX...");
}

void MTX_Reset(void) {
    MTX_nCE_A_Set();
    MTX_nCE_B_Set();
    MTX_nCE_C_Set();
    MTX_nCE_D_Set();
    
    MTX_nRESET_Clear();
    CORETIMER_DelayUs(100);
    MTX_nRESET_Set();
    CORETIMER_DelayUs(300);
}

// take over the pin(s) shared between LCD and MTX
void MTX_Takeover(void) {
    RPC8R = 0;
}

void MTX_SelectDisplay(uint8_t nbr) {
    switch(nbr) {
        case 0:
            MTX_nCE_B_Set();
            MTX_nCE_C_Set();
            MTX_nCE_D_Set();
            MTX_nCE_A_Clear();
            break;
        case 1:
            MTX_nCE_C_Set();
            MTX_nCE_D_Set();
            MTX_nCE_A_Set();
            MTX_nCE_B_Clear();
            break;
        case 2:
            MTX_nCE_B_Set();
            MTX_nCE_D_Set();
            MTX_nCE_A_Set();
            MTX_nCE_C_Clear();
            break;
        case 3:
            MTX_nCE_A_Set();
            MTX_nCE_B_Set();
            MTX_nCE_C_Set();
            MTX_nCE_D_Clear();
            break;
    }
    
    CORETIMER_DelayUs(10);
}

void MTX_SetData(uint8_t data) {
    GPIO_PinWrite(MTX_DATA0_PIN, data & 0x01);
    GPIO_PinWrite(MTX_DATA1_PIN, data & 0x02);
    GPIO_PinWrite(MTX_DATA2_PIN, data & 0x04);
    GPIO_PinWrite(MTX_DATA3_PIN, data & 0x08);
    GPIO_PinWrite(MTX_DATA4_PIN, data & 0x10);
    GPIO_PinWrite(MTX_DATA5_PIN, data & 0x20);
    GPIO_PinWrite(MTX_DATA6_PIN, data & 0x40);
    GPIO_PinWrite(MTX_DATA7_PIN, data & 0x80);
}

void MTX_SetAddress(uint8_t addr) {
    GPIO_PinWrite(MTX_ADDR0_PIN, addr & 0x01);
    GPIO_PinWrite(MTX_ADDR1_PIN, addr & 0x02);
    GPIO_PinWrite(MTX_ADDR2_PIN, addr & 0x04);
    GPIO_PinWrite(MTX_ADDR3_PIN, addr & 0x08);
    GPIO_PinWrite(MTX_ADDR4_PIN, addr & 0x10);
}

void MTX_SetCharacter(uint8_t position, char value) {
    MTX_SetAddress(MTX_REG_CHAR | position);
    MTX_SetData(value);
    
    CORETIMER_DelayUs(100); // TODO: figure out real setup time
    MTX_nWR_Clear();
    CORETIMER_DelayUs(100); // TODO: figure out real setup time
    MTX_nWR_Set();
    CORETIMER_DelayUs(100); // TODO: figure out real setup time
}

void MTX_SelfTest(void) {
    MTX_SetAddress(MTX_REG_CONTROL);
    MTX_SetData(MTX_CTL_SELFTEST);
    
    CORETIMER_DelayUs(100); // TODO: figure out real setup time
    MTX_nWR_Clear();
    CORETIMER_DelayUs(100); // TODO: figure out real setup time
    MTX_nWR_Set();
    
}

// Write text to the display, space-padded to clear anything there previously
void MTX_WriteClearText(uint8_t display, char *text) {
    char buf[17];

    strcpy(buf, "                ");
    memcpy(buf, text, strlen(text));
    MTX_WriteText(display, buf);
}

void MTX_WriteText(uint8_t display, char *text) {
    uint8_t i;
    uint8_t char_nbr = 0;   // character position on the specific display
    uint8_t cur_display = display;
    
    MTX_Takeover();
    for(i=0; i<strlen(text); i++) {
        if(i%8 == 0) {
            MTX_SelectDisplay(cur_display);
            cur_display++;
            char_nbr = 0;
        }
        MTX_SetCharacter(char_nbr, text[i]);
        char_nbr++;
    }
}
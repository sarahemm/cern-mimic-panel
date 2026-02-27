#include "led.h"

#define SPI_Delay() CORETIMER_DelayUs(1)

struct tlc5955 beam_drvr;
struct tlc5955 misc_drvr;

uint8_t beam_maps[2][24] = {
    {38, 25, 24, 2, 12, 15, 5, 4, 6, 20, 18, 21, 9, 19, 34, 46, 47, 30, 43, 44, 39, 28, 29, 36},
    {37, 27, 41, 40, 42, 32, 31, 45, 35, 33, 11, 10, 23, 22, 7, 8, 3, 16, 17, 14, 13, 0, 1, 26}
};

void LED_Init(void) {
    LED_MCU_SDO_Clear();
    LED_SPI_SCLK_Clear();
    LED_LAT_Clear();
    
    LED_InitDriver(&beam_drvr);
    LED_InitDriver(&misc_drvr);
    
    LED_WriteControl();
    LED_WriteControl();
    
    LED_StartupDance();
}

void LED_InitDriver(struct tlc5955 *drvr) {
    uint8_t data_byte;

    for(data_byte=0; data_byte<48; data_byte++) {
        drvr->gs[data_byte] = 0;
    }
    
    memset(drvr->dc, 0, 42);
    memset(drvr->mc, 0, 2);
    memset(drvr->bc, 0, 4);
    drvr->fc.DSPRPT = 1;
    drvr->fc.TMGRST = 1;
    drvr->fc.RFRESH = 0;
    drvr->fc.ESPWM = 1;
    drvr->fc.LSDVLT = 0;
}

void LED_StartupDance(void) {        
    // do a little "we're alive" dance
    
    // beams
    uint16_t i;
    for(i=0; i<24; i++) {
        LED_Clear_Beam(0);
        LED_Set_Beam(0, i, 65000);
        LED_WriteGS();
        CORETIMER_DelayMs(25);
    }
    LED_Clear_Beam(0);

    for(i=0; i<24; i++) {
        LED_Clear_Beam(1);
        LED_Set_Beam(1, i, 65000);
        LED_WriteGS();
        CORETIMER_DelayMs(25);
    }
    LED_Clear_Beam(1);
    LED_WriteGS();
    
    // injection tunnels
    LED_Clear_Misc();
    LED_Set_Misc(LED_TI2_1, 65000);
    LED_Set_Misc(LED_TI8_1, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_TI2_2, 65000);
    LED_Set_Misc(LED_TI8_2, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_TI2_3, 65000);
    LED_Set_Misc(LED_TI8_3, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);

    // dump
    LED_Clear_Misc();
    LED_Set_Misc(LED_DUMP_TUN_B1_1, 65000);
    LED_Set_Misc(LED_DUMP_TUN_B2_1, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_DUMP_TUN_B1_2, 65000);
    LED_Set_Misc(LED_DUMP_TUN_B2_2, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_DUMP_B1_R, 65000);
    LED_Set_Misc(LED_DUMP_B2_R, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_DUMP_B1_G, 65000);
    LED_Set_Misc(LED_DUMP_B2_G, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_Set_Misc(LED_DUMP_B1_B, 65000);
    LED_Set_Misc(LED_DUMP_B2_B, 65000);
    LED_WriteGS();
    CORETIMER_DelayMs(25);
    LED_Clear_Misc();
    LED_WriteGS();
}

void LED_WriteSingleGS(struct tlc5955 drvr) {
    uint8_t data_byte;
    uint8_t data_bit;

    // send the MSB first, zero to indicate this is GS data rather than control data
    LED_MCU_SDO_Clear();
    LED_Toggle_Clk();
    
    // send 768 bits of GS data (16 bits per channel/LED)
    for(data_byte=0; data_byte<48; data_byte++) {
        for(data_bit=0; data_bit<16; data_bit++) {
            if(((drvr.gs[47-data_byte] >> (15-data_bit)) & 1) == 0) {
                LED_MCU_SDO_Clear();
            } else {
                LED_MCU_SDO_Set();
            }
            LED_Toggle_Clk();
        }
    }
    LED_MCU_SDO_Clear();
}

void LED_WriteGS(void) {
    // write GS data to both drivers, then latch it in
    LED_WriteSingleGS(misc_drvr);
    LED_WriteSingleGS(beam_drvr);
    LED_Toggle_Lat();
}

void LED_WriteControl(void) {
    // write control data to both drivers, then latch it in
    LED_WriteSingleControl(misc_drvr);
    LED_WriteSingleControl(beam_drvr);
    LED_Toggle_Lat();
}

void LED_WriteSingleControl(struct tlc5955 drvr) {
    uint16_t bit;
    
    // send the control flag, 1 = control bits follow
    LED_MCU_SDO_Set();
    LED_Toggle_Clk();
        
    // send the magic "I really want to write control bits" code, 10010110
    LED_MCU_SDO_Set();
    LED_Toggle_Clk();
    LED_MCU_SDO_Clear();
    LED_Toggle_Clk();
    LED_Toggle_Clk();
    LED_MCU_SDO_Set();
    LED_Toggle_Clk();
    LED_MCU_SDO_Clear();
    LED_Toggle_Clk();
    LED_MCU_SDO_Set();
    LED_Toggle_Clk();
    LED_Toggle_Clk();
    LED_MCU_SDO_Clear();
    LED_Toggle_Clk();
    
    // send 389 zeroes (padding between control flag and control data)
    LED_MCU_SDO_Clear();
    for(bit=0; bit<389; bit++) {
        LED_Toggle_Clk();
    }
    
    // send the 5 FC bits
    LED_Set_SDO(drvr.fc.LSDVLT);
    LED_Toggle_Clk();
    LED_Set_SDO(drvr.fc.ESPWM);
    LED_Toggle_Clk();
    LED_Set_SDO(drvr.fc.RFRESH);
    LED_Toggle_Clk();
    LED_Set_SDO(drvr.fc.TMGRST);
    LED_Toggle_Clk();
    LED_Set_SDO(drvr.fc.DSPRPT);
    LED_Toggle_Clk();
    
    // send the 21 GC bits
    // TODO: actually send bits instead of all fixed
    LED_MCU_SDO_Set();
    for(bit=0; bit<21; bit++) {
        LED_Toggle_Clk();
    }
    
    // send the 9 MC bits
    // TODO: actually send bits instead of all zeroes
    LED_MCU_SDO_Clear();
    for(bit=0; bit<9; bit++) {
        LED_Toggle_Clk();
    }
    
    // send the 336 DC bits
    // TODO: actually send bits instead of all fixed
    LED_MCU_SDO_Set();
    for(bit=0; bit<336; bit++) {
        LED_Toggle_Clk();
    }
}

void LED_Toggle_Clk(void) {
    SPI_Delay();
    LED_SPI_SCLK_Set();
    SPI_Delay();
    LED_SPI_SCLK_Clear();
}

void LED_Toggle_Lat(void) {
    SPI_Delay();
    LED_LAT_Set();
    SPI_Delay();
    LED_LAT_Clear();
}

void LED_Set_SDO(bool value) {
    if(value) {
        LED_MCU_SDO_Set();
    } else {
        LED_MCU_SDO_Clear();
    }
}

void LED_Clear_Beam(uint8_t beam) {
    uint8_t led;
    
    for(led=0; led<24; led++) {
        beam_drvr.gs[beam_maps[beam][led]] = 0;
    }
}

void LED_Clear_Misc(void) {
    uint8_t data_byte;
    
    for(data_byte=0; data_byte<48; data_byte++) {
        misc_drvr.gs[data_byte] = 0;
    }
}

void LED_Set_Misc(uint8_t led, uint16_t value) {
    misc_drvr.gs[led] = value;
}

void LED_Set_Beam(uint8_t beam, uint8_t led, uint16_t value) {
    beam_drvr.gs[beam_maps[beam][led]] = value;
}

void LED_Set(uint8_t driver, uint8_t led, uint16_t value) {
    if(driver == LED_DRVR_BEAM_1) {
        LED_Set_Beam(0, led, value);
    } else if(driver == LED_DRVR_BEAM_2) {
        LED_Set_Beam(1, led, value);
    } else if(driver == LED_DRVR_BOTH_BEAMS) {
        LED_Set_Beam(0, led, value);
        LED_Set_Beam(1, led, value);
    } else if(driver == LED_DRVR_MISC) {
        LED_Set_Misc(led, value);
    }
}

void LED_Clear_Drvr(uint8_t driver) {
    uint8_t i;
    
    switch(driver) {
        case LED_DRVR_BEAM_1:
            for(i=0; i<24; i++) {
                beam_drvr.gs[beam_maps[0][i]] = 0;
            }
            break;
        case LED_DRVR_BEAM_2:
            for(i=0; i<24; i++) {
                beam_drvr.gs[beam_maps[1][i]] = 0;
            }
            break;
        case LED_DRVR_BOTH_BEAMS:
            for(i=0; i<24; i++) {
                beam_drvr.gs[beam_maps[0][i]] = 0;
                beam_drvr.gs[beam_maps[1][i]] = 0;
            }
            break;
        case LED_DRVR_MISC:
            // clear only the things that ANIM has access to, not the flags
            misc_drvr.gs[LED_TI2_1] = 0;
            misc_drvr.gs[LED_TI2_2] = 0;
            misc_drvr.gs[LED_TI2_3] = 0;
            misc_drvr.gs[LED_TI8_1] = 0;
            misc_drvr.gs[LED_TI8_2] = 0;
            misc_drvr.gs[LED_TI8_3] = 0;
            misc_drvr.gs[LED_DUMP_TUN_B1_1] = 0;
            misc_drvr.gs[LED_DUMP_TUN_B1_2] = 0;
            misc_drvr.gs[LED_DUMP_TUN_B2_1] = 0;
            misc_drvr.gs[LED_DUMP_TUN_B2_2] = 0;
            misc_drvr.gs[LED_DUMP_B1_R] = 0;
            misc_drvr.gs[LED_DUMP_B1_G] = 0;
            misc_drvr.gs[LED_DUMP_B1_B] = 0;
            misc_drvr.gs[LED_DUMP_B2_R] = 0;
            misc_drvr.gs[LED_DUMP_B2_G] = 0;
            misc_drvr.gs[LED_DUMP_B2_B] = 0;

            misc_drvr.gs[LED_IP1_R] = 0;
            misc_drvr.gs[LED_IP1_G] = 0;
            misc_drvr.gs[LED_IP1_B] = 0;
            misc_drvr.gs[LED_IP2_R] = 0;
            misc_drvr.gs[LED_IP2_G] = 0;
            misc_drvr.gs[LED_IP2_B] = 0;
            misc_drvr.gs[LED_IP5_R] = 0;
            misc_drvr.gs[LED_IP5_G] = 0;
            misc_drvr.gs[LED_IP5_B] = 0;
            misc_drvr.gs[LED_IP8_R] = 0;
            misc_drvr.gs[LED_IP8_G] = 0;
            misc_drvr.gs[LED_IP8_B] = 0;

            break;
    }
}
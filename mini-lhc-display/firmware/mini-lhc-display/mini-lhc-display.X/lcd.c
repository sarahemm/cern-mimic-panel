#include "lcd.h"
#include "lcd-font.h"
#include "link.h"

#include <string.h>
#include <stdio.h> // just for debugging, remove soon

#define LCD_Delay() CORETIMER_DelayUs(10)

#define LCD_WRAP_SOFT   // wrap multiline text at word boundaries not strict 21 characters

void LCD_Init(void) {
    LCD_Takeover();
    LCD_BL_EN_Set();

    // TODO: figure out how long it actually takes to complete a reset
    LCD_nRESET_Clear();
    CORETIMER_DelayMs(100);
    LCD_nRESET_Set();
    CORETIMER_DelayMs(100);
    
    LCD_Send_Command(LCD_CMD_SET_DISPSTART);
    LCD_Send_Command(LCD_CMD_SET_ADC_REVERSE);
    LCD_Send_Command(LCD_CMD_SET_COMMODE_REVERSE);
    LCD_Send_Command(LCD_CMD_SET_DISP_NORMAL);
    LCD_Send_Command(LCD_CMD_SET_BIAS_1_9);
    LCD_Send_Command(LCD_CMD_SET_POWERCTL | LCD_POWERCTL_BOOSTER_ON | LCD_POWERCTL_REGULATOR_ON | LCD_POWERCTL_FOLLOWER_ON);
    LCD_Send_Command(LCD_CMD_SET_BOOSTER_RATIO);
    LCD_Send_Command(LCD_BOOSTER_RATIO_2_3_4);
    LCD_Send_Command(LCD_CMD_SET_V0_RATIO);
    LCD_Send_Command(LCD_CMD_SET_CONTRAST_1);
    LCD_Send_Command(LCD_CMD_SET_CONTRAST_2);
    LCD_Send_Command(LCD_CMD_SET_STATICIND_OFF);
    LCD_Send_Command(LCD_CMD_SET_STATICIND_NOFLASH);
    LCD_Send_Command(LCD_CMD_SET_DISPLAY_ON);
    LCD_StartupDance();
}

void LCD_StartupDance(void) {
    LCD_Checkerboard_Test();
    
    /*
    LCD_Write_Text(7, 0, "Hello, World!");
    LCD_Write_Text(2, 0, "meow meow meow meow");
    LCD_Write_Text(3, 0, "meow meow meow meow");
    LCD_Write_Text(4, 0, "meow meow meow meow");
    
    char *teststrs[] = {
        "This is line 1 meow",
        "This is line 2 meow",
        "This is line 3 meow",
        "This is line 4 meow",
        "This is line 5 meow",
        "This is line 6 meow",
        "This is line 7 meow",
        "This is line 8 meow",
    };
    LCD_Write_Multiline_Text(0, teststrs);
    */
    CORETIMER_DelayMs(500);
    LCD_Clear();
}

// take over the pin(s) shared between LCD and MTX
void LCD_Takeover(void) {
    RPC8R = 4;
}

void LCD_Send_Command(uint8_t command) {
    while(SPI2_IsBusy());
    
    GPIO_PinWrite(LCD_A0_PIN, 0);
    GPIO_PinWrite(LCD_nCS_PIN, 0);
    LCD_Delay();
    SPI2_Write(&command, 1);
    while(SPI2_IsBusy());
    LCD_Delay();
    GPIO_PinWrite(LCD_nCS_PIN, 1);
}

void LCD_Send_Data(uint8_t *data, uint8_t len) {
    while(SPI2_IsBusy());
    
    GPIO_PinWrite(LCD_A0_PIN, 1);
    GPIO_PinWrite(LCD_nCS_PIN, 0);
    LCD_Delay();
    SPI2_Write(data, len);
    while(SPI2_IsBusy());
    LCD_Delay();
    GPIO_PinWrite(LCD_nCS_PIN, 1);    
}

void LCD_Write_Pixels(uint8_t page, uint8_t startcol, uint8_t *data, uint8_t len) {
    LCD_Takeover();
    LCD_Set_Column(startcol);
    LCD_Send_Command(LCD_CMD_SET_PAGEADDR | page);
    LCD_Send_Data(data, len);
}

void LCD_Set_Column(uint8_t col) {
    LCD_Send_Command(LCD_CMD_SET_STARTCOL | col >> 4);
    LCD_Send_Command(col & 0x0F);
}

void LCD_Checkerboard_Test(void) {
    // draw an 8x8 checkerboard across the whole display to test everything
    uint8_t buf[171];
    uint8_t i, j;
    uint8_t value = 0;
    for(j=0; j<8; j++) {
        for(i=0; i<171; i++) {
            if(i%8==0)
                value = value == 0 ? 255 : 0;
            buf[i] = value;
        }
        value = value == 0 ? 255 : 0;
        LCD_Write_Pixels(j, 0, buf, 171);
    }
}

void LCD_Clear(void) {
    uint8_t buf[171];
    uint8_t i, page;
    
    for(i=0; i<170; i++) {
        buf[i] = 0;
    }
    
    for(page=0; page<8; page++) {
        LCD_Write_Pixels(page, 0, buf, 171);
    }
}

void LCD_Write_Text(uint8_t page, uint8_t startcol, char *text) {
    uint8_t char_nbr;
    
    LCD_Takeover();
    LCD_Set_Column(startcol);
    LCD_Send_Command(LCD_CMD_SET_PAGEADDR | page);

    for(char_nbr=0; char_nbr<strlen(text); char_nbr++) {
        // font skips the first 0x1F characters of ASCII since they're non-printable
        // so offset what we're looking for by 20 and use a stride of 6 bytes
        LCD_Send_Data(&font[(text[char_nbr]-0x20)*6], 6);
    }
}

#ifdef LCD_WRAP_SOFT
    void LCD_Write_Multiline_Text(char *text) {
        char *line_start_ptr = text;
        char *text_ptr = text;
        char *last_space_ptr = 0;
        uint8_t line_len = 0;
        uint8_t line_nbr = 0;
        char line_buf[22] = "                     ";
        
        do {
            if(*text_ptr == ' ') last_space_ptr = text_ptr;
            if(*text_ptr == '\n') {
                // newline, so print whatever we have so far
                strcpy(line_buf, "                     ");
                strncpy(line_buf, line_start_ptr, text_ptr - line_start_ptr);
                LCD_Write_Text(7-line_nbr, 0, line_buf);
                line_nbr++;
                line_len = 0;
                line_start_ptr = text_ptr + 1;
            } else if(line_len > 20) {
                // line is now too long, so print from the start of the line to the previous space
                strcpy(line_buf, "                     ");
                strncpy(line_buf, line_start_ptr, last_space_ptr - line_start_ptr);
                LCD_Write_Text(7-line_nbr, 0, line_buf);
                strcpy(line_buf, "                     ");
                line_nbr++;
                // back up the pointer to where the last space was, so we pick back up there
                text_ptr = last_space_ptr;
                line_len = 0;
                line_start_ptr = text_ptr;
            }
            if(line_len == 0 && *text_ptr == ' ') {
                // if the first character on a line is a space, skip over that so we don't start lines with whitespace
                line_start_ptr++;
            }
            line_len++;
            text_ptr++;
        } while((text_ptr < text + strlen(text)) && line_nbr < 8);
        if(line_start_ptr != text_ptr) {
            strcpy(line_buf, "                     ");
            strncpy(line_buf, line_start_ptr, text_ptr - line_start_ptr);
            LCD_Write_Text(7-line_nbr, 0, line_buf);
        }
    }
#else
    void LCD_Write_Multiline_Text(char *text) {
        uint16_t text_idx, line_idx;
        uint8_t line_nbr = 0;
        char line_buf[22];

        //sprintf(line_buf, "len: %d", strlen(text));
        //LCD_Write_Text(0, 0, line_buf);
        //return;

        line_buf[0] = 0;
        line_idx = 0;
        for(text_idx=0; text_idx < strlen(text); text_idx++) {
            if(text[text_idx] == '\n') {
                text[text_idx] = ' ';
                line_idx = 20;
            }
            line_buf[line_idx+1] = 0;
            line_buf[line_idx] = text[text_idx];
            if(line_idx == 20) {
                LCD_Write_Text(7-line_nbr, 0, line_buf);
                line_nbr++;
                if(line_nbr == 8) break;
                line_idx = 0;
                line_buf[0] = 0;
            } else {
                line_idx++;
            }
        }
        if(line_nbr < 8) LCD_Write_Text(7-line_nbr, 0, line_buf);
    }
#endif

void LCD_Write_Debug_Text(uint8_t row, char *text) {
    // if the debug flag isn't set, don't overwrite the LCD with debug messages
    if(!LINK_IsDebugFlagSet()) return;

    LCD_Write_Text(row, 0, text);
}
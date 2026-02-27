/* 
 * File:   lcd.h
 * Author: Sen
 *
 * Created on October 19, 2025, 3:06 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"                // SYS function prototypes

#define LCD_A0_PIN  MTX_DATA7_PIN
#define LCD_nCS_PIN MTX_ADDR1_PIN

#define LCD_CMD_SET_DISPSTART           0x40
#define LCD_CMD_SET_ADC_NORMAL          0xA0
#define LCD_CMD_SET_ADC_REVERSE         0xA1
#define LCD_CMD_SET_COMMODE_NORMAL      0xC0
#define LCD_CMD_SET_COMMODE_REVERSE     0xC8
#define LCD_CMD_SET_DISP_NORMAL         0xA6
#define LCD_CMD_SET_DISP_REVERSE        0xA7
#define LCD_CMD_SET_BIAS_1_9            0xA1
#define LCD_CMD_SET_BIAS_1_7            0xA2
#define LCD_CMD_SET_POWERCTL            0x28
#define LCD_POWERCTL_BOOSTER_ON         0x04
#define LCD_POWERCTL_REGULATOR_ON       0x02
#define LCD_POWERCTL_FOLLOWER_ON        0x01
#define LCD_CMD_SET_BOOSTER_RATIO       0xF8
#define LCD_BOOSTER_RATIO_2_3_4         0x00
#define LCD_BOOSTER_RATIO_5             0x01
#define LCD_BOOSTER_RATIO_6             0x03
#define LCD_CMD_SET_V0_RATIO            0x27
#define LCD_CMD_SET_CONTRAST_1          0x81    // TODO: figure out setting other contrast values
#define LCD_CMD_SET_CONTRAST_2          0x16
#define LCD_CMD_SET_STATICIND_OFF       0xAC
#define LCD_CMD_SET_STATICIND_ON        0xAD
#define LCD_CMD_SET_STATICIND_NOFLASH   0x00
#define LCD_CMD_SET_STATICIND_FLASH     0x01
#define LCD_CMD_SET_DISPLAY_OFF         0xAE
#define LCD_CMD_SET_DISPLAY_ON          0xAF
#define LCD_CMD_SET_PAGEADDR            0xB0
#define LCD_CMD_SET_STARTCOL            0x10

    void LCD_Init(void);
    void LCD_StartupDance(void);
    void LCD_Takeover(void);
    void LCD_Send_Command(uint8_t);
    void LCD_Send_Data(uint8_t *, uint8_t);
    void LCD_Write_Pixels(uint8_t, uint8_t, uint8_t *, uint8_t);
    void LCD_Clear(void);
    void LCD_Set_Column(uint8_t);
    void LCD_Checkerboard_Test(void);
    void LCD_Write_Text(uint8_t, uint8_t, char *);
    void LCD_Write_Multiline_Text(char *);
    void LCD_Write_Debug_Text(uint8_t, char *);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */


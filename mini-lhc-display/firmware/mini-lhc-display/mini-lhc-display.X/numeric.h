/* 
 * File:   numeric.h
 * Author: Sen
 *
 * Created on September 30, 2025, 8:06 AM
 */

#ifndef NUMERIC_H
#define	NUMERIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "definitions.h"                // SYS function prototypes

    void NUM_Init(void);
    void NUM_WriteRegister(uint8_t reg, uint8_t value);
    void NUM_SetIntensity(uint8_t intensity);
    void NUM_SetDecode(uint8_t mode);
    void NUM_SetFont(uint8_t font);
    void NUM_WriteDigit(uint8_t, uint8_t);
 
#define NUM_I2C_ADDR          0x00

#define NUM_REG_DIGIT0        0x01
#define NUM_REG_DIGIT1        0x02
#define NUM_REG_DIGIT2        0x03
#define NUM_REG_DIGIT3        0x04
#define NUM_REG_DIGIT4        0x05
#define NUM_REG_DIGIT5        0x06
#define NUM_REG_DIGIT6        0x07
#define NUM_REG_DIGIT7        0x08
  
#define NUM_REG_DECODE_MODE   0x09
#define NUM_REG_GLOBAL_INTEN  0x0A
#define NUM_REG_SCAN_LIMIT    0x0B
#define NUM_REG_SHUTDOWN      0x0C
#define NUM_REG_SELF_ADDR     0x2D
#define NUM_REG_FEATURE       0x0E
#define NUM_REG_DISP_TEST     0x0F

#define NUM_REG_DIGIT01_INTEN 0x10
#define NUM_REG_DIGIT23_INTEN 0x11
#define NUM_REG_DIGIT45_INTEN 0x12
#define NUM_REG_DIGIT67_INTEN 0x13

#define NUM_REG_DIGIT0_DIAG   0x14
#define NUM_REG_DIGIT1_DIAG   0x15
#define NUM_REG_DIGIT2_DIAG   0x16
#define NUM_REG_DIGIT3_DIAG   0x17
#define NUM_REG_DIGIT4_DIAG   0x18
#define NUM_REG_DIGIT5_DIAG   0x19
#define NUM_REG_DIGIT6_DIAG   0x1A
#define NUM_REG_DIGIT7_DIAG   0x1B

#define NUM_DECODE_RAW        0x00
#define NUM_DECODE_FONT       0x01
#define NUM_DECODE_ALL_RAW    0x00
#define NUM_DECODE_ALL_FONT   0xFF

#define NUM_FONT_CODEB        0x00
#define NUM_FONT_HEX          0x01

#define NUM_REG_FEATURE_EXTCLK      0x00
#define NUM_REG_FEATURE_RESET       0x01
#define NUM_REG_FEATURE_FONT        0x02
#define NUM_REG_FEATURE_BLINK       0x04
#define NUM_REG_FEATURE_BLINKFREQ   0x05
#define NUM_REG_FEATURE_BLINKSYNC   0x06
#define NUM_REG_FEATURE_BLINKSTART  0x07

#define NUM_REG_SHUTDOWN_SHUTDOWN             0x00
#define NUM_REG_SHUTDOWN_RUNNING              0x01
#define NUM_REG_SHUTDOWN_RESET_FEATUREREG     0x00
#define NUM_REG_SHUTDOWN_PRESERVE_FEATUREREG  0x80

#define NUM_DP_OFF  0x00
#define NUM_DP_ON   0x01


#ifdef	__cplusplus
}
#endif

#endif	/* NUMERIC_H */


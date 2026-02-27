/* 
 * File:   matrix.h
 * Author: Sen
 *
 * Created on September 29, 2025, 5:39 PM
 */

#ifndef MATRIX_H
#define	MATRIX_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "definitions.h"                // SYS function prototypes

#define DISP_ACCELMODE  0
#define DISP_BEAMMODE   2
    
    void MTX_Init(void);
    void MTX_Reset(void);
    void MTX_Takeover(void);
    void MTX_SelectDisplay(uint8_t nbr);
    void MTX_SelfTest(void);
    void MTX_SetData(uint8_t data);
    void MTX_SetAddress(uint8_t addr);
    void MTX_SetCharacter(uint8_t position, char value);
    
    void MTX_WriteClearText(uint8_t, char *);
    void MTX_WriteText(uint8_t, char *);
    
#define MTX_REG_CHAR        0x18
#define MTX_REG_CONTROL     0x10

#define MTX_CTL_BRIGHT      0x00
#define MTX_CTL_FLASH       0x10
#define MTX_CTL_BLINK       0x20
#define MTX_CTL_SELFTEST    0x40
#define MTX_CTL_CLEAR       0x80

#ifdef	__cplusplus
}
#endif

#endif	/* MATRIX_H */


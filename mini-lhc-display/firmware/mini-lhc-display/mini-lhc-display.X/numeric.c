#include "numeric.h"
#include "matrix.h"

static void NUM_I2CCallback(uintptr_t contextHandle) {
    if (I2C1_ErrorGet() == I2C_ERROR_NONE) {
        //MTX_SetCharacter(1, contextHandle + 4 + 48);
    } else {
        //MTX_SetCharacter(1, contextHandle + 5 + 48);
    }
}

void NUM_Init(void) {
    // set up our I2C callback
    I2C1_CallbackRegister(NUM_I2CCallback, 0);
    
    // bring the AS1115 out of shutdown
    NUM_WriteRegister(NUM_REG_SHUTDOWN, NUM_REG_SHUTDOWN_RUNNING | NUM_REG_SHUTDOWN_RESET_FEATUREREG);
    
    // display all digits, full brightness, decoded using the hex font
    NUM_WriteRegister(NUM_REG_SCAN_LIMIT, 0x07);
    NUM_SetIntensity(15);
    NUM_SetDecode(NUM_DECODE_ALL_RAW);
    //NUM_SetFont(NUM_FONT_HEX);
    
    // display a test digit
    /*
    NUM_WriteRegister(NUM_REG_DIGIT0, 0);
    NUM_WriteRegister(NUM_REG_DIGIT1, 1);
    NUM_WriteRegister(NUM_REG_DIGIT2, 2);
    NUM_WriteRegister(NUM_REG_DIGIT3, 3);
    NUM_WriteRegister(NUM_REG_DIGIT4, 4);
    NUM_WriteRegister(NUM_REG_DIGIT5, 5);
    NUM_WriteRegister(NUM_REG_DIGIT6, 6);
    NUM_WriteRegister(NUM_REG_DIGIT7, 7);
    */
    uint8_t i;
    for(i=0; i<8; i++) {
        CORETIMER_DelayMs(50);
        NUM_WriteRegister(NUM_REG_DIGIT0, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT1, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT2, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT3, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT4, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT5, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT6, 1<<i);
        NUM_WriteRegister(NUM_REG_DIGIT7, 1<<i);
    }
    NUM_SetDecode(NUM_DECODE_ALL_FONT);
    for(i=0; i<=9; i++) {
        CORETIMER_DelayMs(50);
        NUM_WriteRegister(NUM_REG_DIGIT0, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT1, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT2, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT3, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT4, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT5, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT6, 9-i);
        NUM_WriteRegister(NUM_REG_DIGIT7, 9-i);
    }
    for(i=0; i<8; i++) {
        NUM_WriteDigit(i, ' ');
    }
}

void NUM_WriteRegister(uint8_t reg, uint8_t value) {
    uint8_t buf[2];
    
    buf[0] = reg;
    buf[1] = value;

    while(I2C1_IsBusy( ));
    I2C1_Write(NUM_I2C_ADDR, buf, 2);
    while(I2C1_IsBusy( ));
}

void NUM_SetIntensity(uint8_t intensity) {
    NUM_WriteRegister(NUM_REG_GLOBAL_INTEN, 15);
}

void NUM_SetDecode(uint8_t mode) {
    NUM_WriteRegister(NUM_REG_DECODE_MODE, mode);
}

//void NUM_SetFont(uint8_t font) {
//    NUM_WriteRegister(NUM_REG_FEATURE, NUM_REG_FEATURE_FONT, font);
//}

void NUM_WriteDigit(uint8_t digit, uint8_t value) {
    if(value == 0x20 || value == 0xA0) {
        // a space
        NUM_WriteRegister(digit + 1, 0x0F);
    } else {
        // a normal digit, write it out
        NUM_WriteRegister(digit + 1, value);
    }
}
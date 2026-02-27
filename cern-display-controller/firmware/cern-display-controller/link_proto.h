#define CMD_NOOP                0x00
#define CMD_SET_SYSFLAG         0x01
#define CMD_CLEAR_SYSFLAG       0x02

#define CMD_SET_LED             0x10
#define CMD_SET_LED_2CH         0x11
#define CMD_SET_LED_3CH         0x12
#define CMD_SET_LED_MULTI       0x13
#define CMD_SET_LED_2CH_MULTI   0x14
#define CMD_SET_LED_3CH_MULTI   0x15

#define CMD_SET_NUMERIC         0x20
#define CMD_SET_TEXT            0x21
#define CMD_INIT_TEXT_BUFFER    0x22
#define CMD_FILL_TEXT_BUFFER    0x23
#define CMD_SET_TEXT_FROM_BUFFER 0x24

#define CMD_ANIMATION_PLAY      0x30
#define CMD_ANIMATION_STOP      0x31
#define CMD_ANIMATION_MASK_SET    0x32
#define CMD_ANIMATION_MASK_REMOVE 0x33

#define SYSFLAG_DEBUG           0x01

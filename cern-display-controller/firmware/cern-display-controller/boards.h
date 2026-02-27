#define BOARD_MINILHC  0x10

/*****
Mini LHC Display
*****/

#define MINILHC_ENERGY        0x0000
#define MINILHC_TSB           0x0001
#define MINILHC_ACCELMODE     0x0010
#define MINILHC_BEAMMODE      0x0011
#define MINILHC_COMMENT       0x0012

// Wrapup LEDs
#define MINILHC_CRYO_WRAPUP   0x0020
#define MINILHC_COLLIMATOR_B1 0x0021
#define MINILHC_COLLIMATOR_B2 0x0022

// BIS/SMP LEDs
#define MINILHC_BEAMPERMIT_B1 0x0030
#define MINILHC_STABLEBEAM_B1 0x0031
#define MINILHC_MOVABLES_B1   0x0032
#define MINILHC_BEAMPERMIT_B2 0x0033
#define MINILHC_STABLEBEAM_B2 0x0034
#define MINILHC_MOVABLES_B2   0x0035
#define MINILHC_PERMITSLINKED 0x0036

// Injection Tunnel LEDs
#define MINILHC_TI2_FIRST     0x0040  // 3 total
#define MINILHC_TI8_FIRST     0x0043  // 3 total

// Dump LEDs
#define MINILHC_B1_DUMP_FIRST 0x0050  // 2 total
#define MINILHC_B1_DUMP       0x0052
#define MINILHC_B2_DUMP_FIRST 0x0053  // 2 total
#define MINILHC_B2_DUMP       0x0055

// IP LEDs
#define MINILHC_IP1           0x0060
#define MINILHC_IP2           0x0061
#define MINILHC_IP5           0x0062
#define MINILHC_IP8           0x0063

// Beam LEDs
#define MINILHC_B1_FIRST      0x0070  // 24 total, 0x0070 - 0x0088
#define MINILHC_B2_FIRST      0x0090  // 24 total, 0x0090 - 0x00A8

/*****
Mini LHC Display Animations and Masks
*****/

#define MINILHC_ANIM_BOTH_BEAMS     3
#define MINILHC_ANIM_BOTH_BEAMS_EXP 4
#define MINILHC_ANIM_INJECTING      5

#define MINILHC_ANIM_MASK_BEAM_1        0x01    // hide beam 1
#define MINILHC_ANIM_MASK_BEAM_2        0x02    // hide beam 2
#define MINILHC_ANIM_MASK_IP_1          0x04    // hide interactions at ATLAS
#define MINILHC_ANIM_MASK_IP_2          0x08    // hide interactions at ALICE
#define MINILHC_ANIM_MASK_IP_5          0x10    // hide interactions at CMS
#define MINILHC_ANIM_MASK_IP_8          0x20    // hide interactions at LHCb


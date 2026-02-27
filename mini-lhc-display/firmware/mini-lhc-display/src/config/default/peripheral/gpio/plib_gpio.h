/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for MTX_DATA6 pin ***/
#define MTX_DATA6_Set()               (LATCSET = (1<<6))
#define MTX_DATA6_Clear()             (LATCCLR = (1<<6))
#define MTX_DATA6_Toggle()            (LATCINV= (1<<6))
#define MTX_DATA6_OutputEnable()      (TRISCCLR = (1<<6))
#define MTX_DATA6_InputEnable()       (TRISCSET = (1<<6))
#define MTX_DATA6_Get()               ((PORTC >> 6) & 0x1)
#define MTX_DATA6_GetLatch()          ((LATC >> 6) & 0x1)
#define MTX_DATA6_PIN                  GPIO_PIN_RC6

/*** Macros for MTX_DATA7 pin ***/
#define MTX_DATA7_Set()               (LATCSET = (1<<7))
#define MTX_DATA7_Clear()             (LATCCLR = (1<<7))
#define MTX_DATA7_Toggle()            (LATCINV= (1<<7))
#define MTX_DATA7_OutputEnable()      (TRISCCLR = (1<<7))
#define MTX_DATA7_InputEnable()       (TRISCSET = (1<<7))
#define MTX_DATA7_Get()               ((PORTC >> 7) & 0x1)
#define MTX_DATA7_GetLatch()          ((LATC >> 7) & 0x1)
#define MTX_DATA7_PIN                  GPIO_PIN_RC7

/*** Macros for MTX_ADDR0 pin ***/
#define MTX_ADDR0_Set()               (LATCSET = (1<<8))
#define MTX_ADDR0_Clear()             (LATCCLR = (1<<8))
#define MTX_ADDR0_Toggle()            (LATCINV= (1<<8))
#define MTX_ADDR0_OutputEnable()      (TRISCCLR = (1<<8))
#define MTX_ADDR0_InputEnable()       (TRISCSET = (1<<8))
#define MTX_ADDR0_Get()               ((PORTC >> 8) & 0x1)
#define MTX_ADDR0_GetLatch()          ((LATC >> 8) & 0x1)
#define MTX_ADDR0_PIN                  GPIO_PIN_RC8

/*** Macros for MTX_ADDR1 pin ***/
#define MTX_ADDR1_Set()               (LATCSET = (1<<9))
#define MTX_ADDR1_Clear()             (LATCCLR = (1<<9))
#define MTX_ADDR1_Toggle()            (LATCINV= (1<<9))
#define MTX_ADDR1_OutputEnable()      (TRISCCLR = (1<<9))
#define MTX_ADDR1_InputEnable()       (TRISCSET = (1<<9))
#define MTX_ADDR1_Get()               ((PORTC >> 9) & 0x1)
#define MTX_ADDR1_GetLatch()          ((LATC >> 9) & 0x1)
#define MTX_ADDR1_PIN                  GPIO_PIN_RC9

/*** Macros for LCD_nRESET pin ***/
#define LCD_nRESET_Set()               (LATBSET = (1<<10))
#define LCD_nRESET_Clear()             (LATBCLR = (1<<10))
#define LCD_nRESET_Toggle()            (LATBINV= (1<<10))
#define LCD_nRESET_OutputEnable()      (TRISBCLR = (1<<10))
#define LCD_nRESET_InputEnable()       (TRISBSET = (1<<10))
#define LCD_nRESET_Get()               ((PORTB >> 10) & 0x1)
#define LCD_nRESET_GetLatch()          ((LATB >> 10) & 0x1)
#define LCD_nRESET_PIN                  GPIO_PIN_RB10

/*** Macros for MTX_nCE_B pin ***/
#define MTX_nCE_B_Set()               (LATBSET = (1<<11))
#define MTX_nCE_B_Clear()             (LATBCLR = (1<<11))
#define MTX_nCE_B_Toggle()            (LATBINV= (1<<11))
#define MTX_nCE_B_OutputEnable()      (TRISBCLR = (1<<11))
#define MTX_nCE_B_InputEnable()       (TRISBSET = (1<<11))
#define MTX_nCE_B_Get()               ((PORTB >> 11) & 0x1)
#define MTX_nCE_B_GetLatch()          ((LATB >> 11) & 0x1)
#define MTX_nCE_B_PIN                  GPIO_PIN_RB11

/*** Macros for MTX_nCE_A pin ***/
#define MTX_nCE_A_Set()               (LATBSET = (1<<12))
#define MTX_nCE_A_Clear()             (LATBCLR = (1<<12))
#define MTX_nCE_A_Toggle()            (LATBINV= (1<<12))
#define MTX_nCE_A_OutputEnable()      (TRISBCLR = (1<<12))
#define MTX_nCE_A_InputEnable()       (TRISBSET = (1<<12))
#define MTX_nCE_A_Get()               ((PORTB >> 12) & 0x1)
#define MTX_nCE_A_GetLatch()          ((LATB >> 12) & 0x1)
#define MTX_nCE_A_PIN                  GPIO_PIN_RB12

/*** Macros for LED_GSCLK pin ***/
#define LED_GSCLK_Get()               ((PORTB >> 13) & 0x1)
#define LED_GSCLK_GetLatch()          ((LATB >> 13) & 0x1)
#define LED_GSCLK_PIN                  GPIO_PIN_RB13

/*** Macros for MTX_ADDR2 pin ***/
#define MTX_ADDR2_Set()               (LATASET = (1<<10))
#define MTX_ADDR2_Clear()             (LATACLR = (1<<10))
#define MTX_ADDR2_Toggle()            (LATAINV= (1<<10))
#define MTX_ADDR2_OutputEnable()      (TRISACLR = (1<<10))
#define MTX_ADDR2_InputEnable()       (TRISASET = (1<<10))
#define MTX_ADDR2_Get()               ((PORTA >> 10) & 0x1)
#define MTX_ADDR2_GetLatch()          ((LATA >> 10) & 0x1)
#define MTX_ADDR2_PIN                  GPIO_PIN_RA10

/*** Macros for MTX_nWR pin ***/
#define MTX_nWR_Set()               (LATASET = (1<<7))
#define MTX_nWR_Clear()             (LATACLR = (1<<7))
#define MTX_nWR_Toggle()            (LATAINV= (1<<7))
#define MTX_nWR_OutputEnable()      (TRISACLR = (1<<7))
#define MTX_nWR_InputEnable()       (TRISASET = (1<<7))
#define MTX_nWR_Get()               ((PORTA >> 7) & 0x1)
#define MTX_nWR_GetLatch()          ((LATA >> 7) & 0x1)
#define MTX_nWR_PIN                  GPIO_PIN_RA7

/*** Macros for LED_SPI_SCLK pin ***/
#define LED_SPI_SCLK_Set()               (LATBSET = (1<<14))
#define LED_SPI_SCLK_Clear()             (LATBCLR = (1<<14))
#define LED_SPI_SCLK_Toggle()            (LATBINV= (1<<14))
#define LED_SPI_SCLK_OutputEnable()      (TRISBCLR = (1<<14))
#define LED_SPI_SCLK_InputEnable()       (TRISBSET = (1<<14))
#define LED_SPI_SCLK_Get()               ((PORTB >> 14) & 0x1)
#define LED_SPI_SCLK_GetLatch()          ((LATB >> 14) & 0x1)
#define LED_SPI_SCLK_PIN                  GPIO_PIN_RB14

/*** Macros for LCD_SPI_SCLK pin ***/
#define LCD_SPI_SCLK_Get()               ((PORTB >> 15) & 0x1)
#define LCD_SPI_SCLK_GetLatch()          ((LATB >> 15) & 0x1)
#define LCD_SPI_SCLK_PIN                  GPIO_PIN_RB15

/*** Macros for MTX_nCE_C pin ***/
#define MTX_nCE_C_Set()               (LATASET = (1<<0))
#define MTX_nCE_C_Clear()             (LATACLR = (1<<0))
#define MTX_nCE_C_Toggle()            (LATAINV= (1<<0))
#define MTX_nCE_C_OutputEnable()      (TRISACLR = (1<<0))
#define MTX_nCE_C_InputEnable()       (TRISASET = (1<<0))
#define MTX_nCE_C_Get()               ((PORTA >> 0) & 0x1)
#define MTX_nCE_C_GetLatch()          ((LATA >> 0) & 0x1)
#define MTX_nCE_C_PIN                  GPIO_PIN_RA0

/*** Macros for MTX_nCE_D pin ***/
#define MTX_nCE_D_Set()               (LATASET = (1<<1))
#define MTX_nCE_D_Clear()             (LATACLR = (1<<1))
#define MTX_nCE_D_Toggle()            (LATAINV= (1<<1))
#define MTX_nCE_D_OutputEnable()      (TRISACLR = (1<<1))
#define MTX_nCE_D_InputEnable()       (TRISASET = (1<<1))
#define MTX_nCE_D_Get()               ((PORTA >> 1) & 0x1)
#define MTX_nCE_D_GetLatch()          ((LATA >> 1) & 0x1)
#define MTX_nCE_D_PIN                  GPIO_PIN_RA1

/*** Macros for MTX_DATA0 pin ***/
#define MTX_DATA0_Set()               (LATCSET = (1<<0))
#define MTX_DATA0_Clear()             (LATCCLR = (1<<0))
#define MTX_DATA0_Toggle()            (LATCINV= (1<<0))
#define MTX_DATA0_OutputEnable()      (TRISCCLR = (1<<0))
#define MTX_DATA0_InputEnable()       (TRISCSET = (1<<0))
#define MTX_DATA0_Get()               ((PORTC >> 0) & 0x1)
#define MTX_DATA0_GetLatch()          ((LATC >> 0) & 0x1)
#define MTX_DATA0_PIN                  GPIO_PIN_RC0

/*** Macros for MTX_DATA1 pin ***/
#define MTX_DATA1_Set()               (LATCSET = (1<<1))
#define MTX_DATA1_Clear()             (LATCCLR = (1<<1))
#define MTX_DATA1_Toggle()            (LATCINV= (1<<1))
#define MTX_DATA1_OutputEnable()      (TRISCCLR = (1<<1))
#define MTX_DATA1_InputEnable()       (TRISCSET = (1<<1))
#define MTX_DATA1_Get()               ((PORTC >> 1) & 0x1)
#define MTX_DATA1_GetLatch()          ((LATC >> 1) & 0x1)
#define MTX_DATA1_PIN                  GPIO_PIN_RC1

/*** Macros for MTX_DATA2 pin ***/
#define MTX_DATA2_Set()               (LATCSET = (1<<2))
#define MTX_DATA2_Clear()             (LATCCLR = (1<<2))
#define MTX_DATA2_Toggle()            (LATCINV= (1<<2))
#define MTX_DATA2_OutputEnable()      (TRISCCLR = (1<<2))
#define MTX_DATA2_InputEnable()       (TRISCSET = (1<<2))
#define MTX_DATA2_Get()               ((PORTC >> 2) & 0x1)
#define MTX_DATA2_GetLatch()          ((LATC >> 2) & 0x1)
#define MTX_DATA2_PIN                  GPIO_PIN_RC2

/*** Macros for MTX_nFLASH pin ***/
#define MTX_nFLASH_Set()               (LATASET = (1<<2))
#define MTX_nFLASH_Clear()             (LATACLR = (1<<2))
#define MTX_nFLASH_Toggle()            (LATAINV= (1<<2))
#define MTX_nFLASH_OutputEnable()      (TRISACLR = (1<<2))
#define MTX_nFLASH_InputEnable()       (TRISASET = (1<<2))
#define MTX_nFLASH_Get()               ((PORTA >> 2) & 0x1)
#define MTX_nFLASH_GetLatch()          ((LATA >> 2) & 0x1)
#define MTX_nFLASH_PIN                  GPIO_PIN_RA2

/*** Macros for MTX_nRD pin ***/
#define MTX_nRD_Set()               (LATASET = (1<<3))
#define MTX_nRD_Clear()             (LATACLR = (1<<3))
#define MTX_nRD_Toggle()            (LATAINV= (1<<3))
#define MTX_nRD_OutputEnable()      (TRISACLR = (1<<3))
#define MTX_nRD_InputEnable()       (TRISASET = (1<<3))
#define MTX_nRD_Get()               ((PORTA >> 3) & 0x1)
#define MTX_nRD_GetLatch()          ((LATA >> 3) & 0x1)
#define MTX_nRD_PIN                  GPIO_PIN_RA3

/*** Macros for MTX_ADDR4 pin ***/
#define MTX_ADDR4_Set()               (LATASET = (1<<8))
#define MTX_ADDR4_Clear()             (LATACLR = (1<<8))
#define MTX_ADDR4_Toggle()            (LATAINV= (1<<8))
#define MTX_ADDR4_OutputEnable()      (TRISACLR = (1<<8))
#define MTX_ADDR4_InputEnable()       (TRISASET = (1<<8))
#define MTX_ADDR4_Get()               ((PORTA >> 8) & 0x1)
#define MTX_ADDR4_GetLatch()          ((LATA >> 8) & 0x1)
#define MTX_ADDR4_PIN                  GPIO_PIN_RA8

/*** Macros for LCD_BL_EN pin ***/
#define LCD_BL_EN_Set()               (LATBSET = (1<<4))
#define LCD_BL_EN_Clear()             (LATBCLR = (1<<4))
#define LCD_BL_EN_Toggle()            (LATBINV= (1<<4))
#define LCD_BL_EN_OutputEnable()      (TRISBCLR = (1<<4))
#define LCD_BL_EN_InputEnable()       (TRISBSET = (1<<4))
#define LCD_BL_EN_Get()               ((PORTB >> 4) & 0x1)
#define LCD_BL_EN_GetLatch()          ((LATB >> 4) & 0x1)
#define LCD_BL_EN_PIN                  GPIO_PIN_RB4

/*** Macros for MTX_nRESET pin ***/
#define MTX_nRESET_Set()               (LATASET = (1<<4))
#define MTX_nRESET_Clear()             (LATACLR = (1<<4))
#define MTX_nRESET_Toggle()            (LATAINV= (1<<4))
#define MTX_nRESET_OutputEnable()      (TRISACLR = (1<<4))
#define MTX_nRESET_InputEnable()       (TRISASET = (1<<4))
#define MTX_nRESET_Get()               ((PORTA >> 4) & 0x1)
#define MTX_nRESET_GetLatch()          ((LATA >> 4) & 0x1)
#define MTX_nRESET_PIN                  GPIO_PIN_RA4

/*** Macros for MTX_ADDR3 pin ***/
#define MTX_ADDR3_Set()               (LATASET = (1<<9))
#define MTX_ADDR3_Clear()             (LATACLR = (1<<9))
#define MTX_ADDR3_Toggle()            (LATAINV= (1<<9))
#define MTX_ADDR3_OutputEnable()      (TRISACLR = (1<<9))
#define MTX_ADDR3_InputEnable()       (TRISASET = (1<<9))
#define MTX_ADDR3_Get()               ((PORTA >> 9) & 0x1)
#define MTX_ADDR3_GetLatch()          ((LATA >> 9) & 0x1)
#define MTX_ADDR3_PIN                  GPIO_PIN_RA9

/*** Macros for MTX_DATA3 pin ***/
#define MTX_DATA3_Set()               (LATCSET = (1<<3))
#define MTX_DATA3_Clear()             (LATCCLR = (1<<3))
#define MTX_DATA3_Toggle()            (LATCINV= (1<<3))
#define MTX_DATA3_OutputEnable()      (TRISCCLR = (1<<3))
#define MTX_DATA3_InputEnable()       (TRISCSET = (1<<3))
#define MTX_DATA3_Get()               ((PORTC >> 3) & 0x1)
#define MTX_DATA3_GetLatch()          ((LATC >> 3) & 0x1)
#define MTX_DATA3_PIN                  GPIO_PIN_RC3

/*** Macros for MTX_DATA4 pin ***/
#define MTX_DATA4_Set()               (LATCSET = (1<<4))
#define MTX_DATA4_Clear()             (LATCCLR = (1<<4))
#define MTX_DATA4_Toggle()            (LATCINV= (1<<4))
#define MTX_DATA4_OutputEnable()      (TRISCCLR = (1<<4))
#define MTX_DATA4_InputEnable()       (TRISCSET = (1<<4))
#define MTX_DATA4_Get()               ((PORTC >> 4) & 0x1)
#define MTX_DATA4_GetLatch()          ((LATC >> 4) & 0x1)
#define MTX_DATA4_PIN                  GPIO_PIN_RC4

/*** Macros for MTX_DATA5 pin ***/
#define MTX_DATA5_Set()               (LATCSET = (1<<5))
#define MTX_DATA5_Clear()             (LATCCLR = (1<<5))
#define MTX_DATA5_Toggle()            (LATCINV= (1<<5))
#define MTX_DATA5_OutputEnable()      (TRISCCLR = (1<<5))
#define MTX_DATA5_InputEnable()       (TRISCSET = (1<<5))
#define MTX_DATA5_Get()               ((PORTC >> 5) & 0x1)
#define MTX_DATA5_GetLatch()          ((LATC >> 5) & 0x1)
#define MTX_DATA5_PIN                  GPIO_PIN_RC5

/*** Macros for LED_MCU_SDI pin ***/
#define LED_MCU_SDI_Set()               (LATBSET = (1<<5))
#define LED_MCU_SDI_Clear()             (LATBCLR = (1<<5))
#define LED_MCU_SDI_Toggle()            (LATBINV= (1<<5))
#define LED_MCU_SDI_OutputEnable()      (TRISBCLR = (1<<5))
#define LED_MCU_SDI_InputEnable()       (TRISBSET = (1<<5))
#define LED_MCU_SDI_Get()               ((PORTB >> 5) & 0x1)
#define LED_MCU_SDI_GetLatch()          ((LATB >> 5) & 0x1)
#define LED_MCU_SDI_PIN                  GPIO_PIN_RB5

/*** Macros for LED_MCU_SDO pin ***/
#define LED_MCU_SDO_Set()               (LATBSET = (1<<6))
#define LED_MCU_SDO_Clear()             (LATBCLR = (1<<6))
#define LED_MCU_SDO_Toggle()            (LATBINV= (1<<6))
#define LED_MCU_SDO_OutputEnable()      (TRISBCLR = (1<<6))
#define LED_MCU_SDO_InputEnable()       (TRISBSET = (1<<6))
#define LED_MCU_SDO_Get()               ((PORTB >> 6) & 0x1)
#define LED_MCU_SDO_GetLatch()          ((LATB >> 6) & 0x1)
#define LED_MCU_SDO_PIN                  GPIO_PIN_RB6

/*** Macros for LED_LAT pin ***/
#define LED_LAT_Set()               (LATBSET = (1<<7))
#define LED_LAT_Clear()             (LATBCLR = (1<<7))
#define LED_LAT_Toggle()            (LATBINV= (1<<7))
#define LED_LAT_OutputEnable()      (TRISBCLR = (1<<7))
#define LED_LAT_InputEnable()       (TRISBSET = (1<<7))
#define LED_LAT_Get()               ((PORTB >> 7) & 0x1)
#define LED_LAT_GetLatch()          ((LATB >> 7) & 0x1)
#define LED_LAT_PIN                  GPIO_PIN_RB7


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/
#define    GPIO_PORT_A  (0U)
#define    GPIO_PORT_B  (1U)
#define    GPIO_PORT_C  (2U)
typedef uint32_t GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/
#define    GPIO_PIN_RA0  (0U)
#define    GPIO_PIN_RA1  (1U)
#define    GPIO_PIN_RA2  (2U)
#define    GPIO_PIN_RA3  (3U)
#define    GPIO_PIN_RA4  (4U)
#define    GPIO_PIN_RA7  (7U)
#define    GPIO_PIN_RA8  (8U)
#define    GPIO_PIN_RA9  (9U)
#define    GPIO_PIN_RA10  (10U)
#define    GPIO_PIN_RB0  (16U)
#define    GPIO_PIN_RB1  (17U)
#define    GPIO_PIN_RB2  (18U)
#define    GPIO_PIN_RB3  (19U)
#define    GPIO_PIN_RB4  (20U)
#define    GPIO_PIN_RB5  (21U)
#define    GPIO_PIN_RB6  (22U)
#define    GPIO_PIN_RB7  (23U)
#define    GPIO_PIN_RB8  (24U)
#define    GPIO_PIN_RB9  (25U)
#define    GPIO_PIN_RB10  (26U)
#define    GPIO_PIN_RB11  (27U)
#define    GPIO_PIN_RB12  (28U)
#define    GPIO_PIN_RB13  (29U)
#define    GPIO_PIN_RB14  (30U)
#define    GPIO_PIN_RB15  (31U)
#define    GPIO_PIN_RC0  (32U)
#define    GPIO_PIN_RC1  (33U)
#define    GPIO_PIN_RC2  (34U)
#define    GPIO_PIN_RC3  (35U)
#define    GPIO_PIN_RC4  (36U)
#define    GPIO_PIN_RC5  (37U)
#define    GPIO_PIN_RC6  (38U)
#define    GPIO_PIN_RC7  (39U)
#define    GPIO_PIN_RC8  (40U)
#define    GPIO_PIN_RC9  (41U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE    (-1)

typedef uint32_t GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xFU), (uint32_t)(value) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xFU)) & 0x1U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xFU)) & 0x1U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H

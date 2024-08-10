
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

/**
 * data
**/
#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

/**
 * GPIO config
**/
#define EPD_BUSY_PIN 25     //BUSY	P25	Busy Output Pin (means busy)
#define EPD_RST_PIN  26     //RST	P26	Reset, active-low
#define EPD_DC_PIN   27     //DC	P27	Reset, Data/Demand, low level means to demand, high level means data     
#define EPD_CS_PIN   15     //CS	P15	Chip Select, active-low
#define EPD_SCK_PIN  13     //SCK	P13	SPI CLK, Clock Signal Input
#define EPD_MOSI_PIN 33     //DIN	P14	SPI MOSI, Data Input
                            //VCC	3.3V	Power input
                            //GND	GND	Power ground

#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET 0

/*------------------------------------------------------------------------------------------------------*/
UBYTE DEV_Module_Init(void);
void DEV_SPI_WriteByte(UBYTE data);
void GPIO_Config();

#endif

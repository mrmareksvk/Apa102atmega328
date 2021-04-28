/*
 * apaLIB.cpp
 *
 * Created: 2. 11. 2020 9:12:19
 * Author: marek
 */

#include <avr/io.h>
#define F_CPU 16000000UL  		// MCU CRYSTAL - NECESARY FOR DELAY OPERATION
#include <util/delay.h>
#include "apaLIB.h"

#define SET(x,y) x |= (1 << y)

#define SPI_DDR DDRB
#define SS		PINB2
#define MOSI    PINB3
#define SCK     PINB5

/*CONSTRUCTOR*/
LEDcontrol::LEDcontrol(uint8_t ledAmount){
	LEDS = ledAmount;
	rgb LEDcolors [LEDS];		// GENERATES STRUCT ARR FOR SET AMOUNT OF LEDS
	for(short i = 0; i < LEDS; i++){
		LEDcolors[i].red = 0;
		LEDcolors[i].green = 0;
		LEDcolors[i].blue = 0;
	}
	initSPI();
}

/*PUBLIC METHODS*/
void LEDcontrol::initSPI(void){
	// set SS, MOSI and SCK as an output
	SET(SPI_DDR, SS);
	SET(SPI_DDR, SCK);
	SET(SPI_DDR, MOSI);

	// enable SPI, set as master, and clock to fosc/128 (0b01010011)
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
	setAll(0,0,0,224); 		// DEFAULT ALL LEDS OFF
}
/* setSingle method can be called any number of times to change specific one LED without overwriting other values which were set by setSequence/setAll methods*/
void LEDcontrol::setSingle(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, uint8_t LEDpos){
	startFrame();
	for(uint8_t i = 1; i <= LEDS; i++){
		if(i == LEDpos){
			sendFrame(brightness);
			sendFrame(blue);
			LEDcolors[i].blue = blue;
			sendFrame(green);
			LEDcolors[i].green = green;
			sendFrame(red);
			LEDcolors[i].red = red;
			break;
		}else{
			sendFrame(brightness);
			sendFrame(LEDcolors[i].blue);
			sendFrame(LEDcolors[i].green);
			sendFrame(LEDcolors[i].red);
		}
	}
	stopFrame();
}
/* setSequence method gives you ability to produce led array of any colors of  lengts even smaller than maximum LED strip size */
void LEDcontrol::setSequence(rgb LEDcolors[], uint8_t brightness, uint8_t LEDamount){
	startFrame();
	for(uint8_t i = 0; i <= LEDamount; i++){
		sendFrame(brightness);
		sendFrame(LEDcolors[i].blue);
		sendFrame(LEDcolors[i].green);
		sendFrame(LEDcolors[i].red);
	}
	stopFrame();
}
/* setALl method changes whole LED strip to same color and brightness, changing the amount of LEDS set in constructor during the object creation */
void LEDcontrol::setAll(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness){
	startFrame();
	for(uint8_t i = 1; i <= LEDS; i++){
		LEDcolors[i].red = red;
		LEDcolors[i].green = green;
		LEDcolors[i].blue = blue;
		sendFrame(brightness);
		sendFrame(blue);
		sendFrame(green);
		sendFrame(red);
	}
	stopFrame();
}


/* PRIVATE METHODS*/
/* sendFrame method used for SPI transmission for other methods*/
void LEDcontrol::sendFrame(uint8_t data){
	SPDR = data;					// load into the register
	while(!(SPSR & (1 << SPIF)));	// wait for transmission complete
}
/* starFrame method produces 4 bytes (32bit) of 0 - recognized by APA102 as start of the controling sequence*/
void LEDcontrol::startFrame(void){
	for(uint8_t i = 0; i < 4; i++){
		sendFrame(0);
	}
}
/* stopFrame method is almost same as startFrame but producing 4 bytes of 1's and is recognized as end of controlling */
void LEDcontrol::stopFrame(void){
	for(uint8_t i = 0; i < 4; i++){
		sendFrame(1);
	}
}

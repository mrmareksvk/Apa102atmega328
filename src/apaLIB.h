/*
 * apaLIB.h
 *
 * Created: 2. 11. 2020 9:12:34
 *  Author: marek
 */
// HEADER FILE FOR apaLIB, library used for controlling of AP102 LED's with ATMEGA328 controller with 16MHz crystal
#ifndef APALIB_H_
#define APALIB_H_

#include <avr/io.h>

typedef struct rgb
{
	uint8_t red, green, blue;
} rgb;


class LEDcontrol
{
	public:
		LEDcontrol(uint8_t ledAmount); 				// Constructor
		void setAll(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 225);
		void setSingle(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 225, uint8_t LEDpos = 0);
		void setSequence(rgb LEDcolors[], uint8_t brightness = 225, uint8_t LEDamount = 1);
	private:
		void initSPI(void);
		void sendFrame(uint8_t data);
		void startFrame(void);
		void stopFrame(void);
		uint8_t LEDS;
		rgb LEDcolors [];
	};


#endif /* APALIB_H_ */

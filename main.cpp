/*
** ILI9341 Library demo
** Copyright (C) 2009 - 2015 Radu Motisan, radu.motisan@gmail.com
**
** This file is a part of "ILI9341" open source library.
**
** ILI9341 Library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 3 of the License,
** or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "config.h"
// microcontroller interface
#include "gpio/DigitalPin.h"
#include "spi/spi.h"
#include "adc/adc.h"
// lcd
#include "ili9341/ili9341.h"
// various pheripherals
#include "touch_resistive/TouchScreen.h"

// microcontroller
ADC10b				adc;
DigitalPin			pinDC(&PORTC, PC4), pinRESET(&PORTC, PC5), pinCS(&PORTC, PC6),

					pinTouchXN(&PORTF, PF2),pinTouchYP(&PORTF, PF3),
					pinTouchXP(&PORTA, PA0), pinTouchYN(&PORTA, PA1),


					backlight(&PORTA, PA2);

// display and input
ILI9341 			lcd(&pinDC, &pinRESET, &pinCS);
TouchScreen			touch(&adc, &pinTouchXP, &pinTouchYN, &pinTouchXN, &pinTouchYP);



int main(void) {
	spi_init();	// needed by ILI9341

	// init display
	lcd.init();
	lcd.setRotation(ILI9341::ROT0);

	// start
	lcd.drawClear(BLACK);


	while (1) {
		uint16_t x = 0, y = 0, z = 0;

		if (touch.read(&x, &y, &z)) {
			if (y < 100) {
				if (x<120) {
				backlight = 1;
				_delay_us(10);
				backlight = 0;
				} else
					lcd.init();
			}
			else
			lcd.drawPixel(x,y,4,RED);
		}
		if (cmdRefreshText) {
			//backlight.toggle();
			cmdRefreshText = false;
		}
	}
}


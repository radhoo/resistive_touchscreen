/*
    File:       TouchScreen.h
    Version:    0.1.0
    Date:       August, 2015
	License:	GPL v2

	Resistive touchscreen driver

    ****************************************************************************
    Copyright (C) 2015 Radu Motisan  <radu.motisan@gmail.com>

	http://www.pocketmagic.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ****************************************************************************
 */

#pragma once

#include <stdint.h>
#include "../gpio/DigitalPin.h"
#include "../adc/adc.h"

#define TOUCH_HEIGHT			 					240	// touchscreen height in landscape mode
#define TOUCH_WIDTH 								320	// touchscreen width in landscape mode

class TouchScreen {
public:
	enum Orientation {
				PORTRAIT = 0,		// portrait orientation
				LANDSCAPE = 1		// landscape orientation
			};

	/*
	 * TouchScreen constructor
	 * init the GPIO pins and set the orientation (default portrait)
	 * XN and YP are ADC pins, YP and YN are general IO
	 */
	TouchScreen(ADC10b *adc, DigitalPin *xp, DigitalPin *yn, DigitalPin *xn, DigitalPin *yp, Orientation orientation = PORTRAIT, uint16_t r = 0);

	// change the orientation (default portrait)
	void	setOrientation(Orientation orientation = PORTRAIT);

	uint16_t readRawPressure(void);
	uint16_t readRawY();
	uint16_t readRawX();

	bool	read(uint16_t *x, uint16_t *y, uint16_t *pressure);

private:
	ADC10b 		*m_adc;
	DigitalPin 	*xn_adc, *yp_adc, *xp_io, *yn_io;
	uint16_t 	touchResistance;
	Orientation m_orientation;
};

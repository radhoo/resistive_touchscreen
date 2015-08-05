/*
    File:       TouchScreen.cpp
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

#include "TouchScreen.h"

/*
 * TouchScreen constructor
 * init the GPIO pins and set the orientation (default portrait)
 * XN and YP are ADC pins, YP and YN are general IO
 */
TouchScreen::TouchScreen(ADC10b *adc, DigitalPin *xp, DigitalPin *yn, DigitalPin *xn, DigitalPin *yp, Orientation orientation, uint16_t r) {
	m_adc = adc;
	xp_io = xp;
	yn_io = yn;
	xn_adc = xn;
	yp_adc = yp;
	m_orientation = orientation;
	touchResistance = r;
}

// change the orientation (default portrait)
void TouchScreen::setOrientation(Orientation orientation) {
	m_orientation = orientation;
}

uint16_t TouchScreen::readRawX(void) {
	xn_adc->config(DigitalPin::INPUT);
	*xn_adc = 0;

	xp_io->config(DigitalPin::INPUT);
	*xp_io = 0;

	yn_io->config(DigitalPin::OUTPUT);
	*yn_io = 0;

	yp_adc->config(DigitalPin::OUTPUT);
	*yp_adc = 1;

	return 1024 - m_adc->read(xn_adc->getPin()) - 1;
}


uint16_t TouchScreen::readRawY(void) {
	yn_io->config(DigitalPin::INPUT);
	*yn_io = 0;

	yp_adc->config(DigitalPin::INPUT);
	*yp_adc = 0;

	xn_adc->config(DigitalPin::OUTPUT);
	*xn_adc = 0;

	xp_io->config(DigitalPin::OUTPUT);
	*xp_io = 1;

	return 1024 - m_adc->read(yp_adc->getPin()) - 1;
}

uint16_t TouchScreen::readRawPressure(void) {
	// Set X+ to ground
	xp_io->config(DigitalPin::OUTPUT);
	*xp_io = 0;

	// Set Y- to VCC
	yn_io->config(DigitalPin::OUTPUT);
	*yn_io = 1;

	// Hi-Z X- and Y+
	xn_adc->config(DigitalPin::INPUT);
	*xn_adc = 0;

	yp_adc->config(DigitalPin::INPUT);
	*yp_adc = 0;

	int z1 = m_adc->read(xn_adc->getPin());
	int z2 = m_adc->read(yp_adc->getPin());

	if (touchResistance != 0) {
		// now read the x
		float rtouch;
		rtouch = z2;
		rtouch /= z1;
		rtouch -= 1;
		rtouch *= readRawX();
		rtouch *= touchResistance;
		rtouch /= 1024;

		return rtouch;
	} else
		return 1024 - (z2-z1) -1;
}

bool TouchScreen::read(uint16_t *x, uint16_t *y, uint16_t *pressure) {
	uint32_t calX1 = 0, calY1 = 15, calX2 = 890, calY2 = 845;

	uint16_t rawp = readRawPressure();

	if (rawp == 0) return false;
	// two consequent reads to double check coords and reduce errors
	uint16_t rawx = readRawX();
	if (readRawX() != rawx) return false;

	uint16_t rawy = readRawY();
	if (readRawY() != rawy) return false;

	if (rawx < calX1 || rawx > calX2 || rawy < calY1 || rawy > calY2)
			return false;

	rawx = (rawx - calX1) * TOUCH_WIDTH / (calX2 - calX1);
	rawy = (rawy - calY1) * TOUCH_HEIGHT / (calY2 - calY1);

	if (m_orientation == PORTRAIT) {
		*x = rawy;
		*y = TOUCH_WIDTH - rawx;
	} else {
		*x = rawx;
		*y = rawy;
	}

	*pressure = rawp;

	return true;
}

/*
    File:       adc.h
    Version:    0.1.0
    Date:       July, 2015
	License:	GPL v2

	Analog to digital 10bit class

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

#include <avr/io.h>
#include "../config.h"

class ADC10b {
	bool inited = false;
public:
	void init();
	// Reads the ADC port specified by i
	uint16_t read(uint8_t i);
	// return ADC port voltage computed against given VRef, with resistive divider
	uint16_t readDivVoltage(float vref, uint16_t divider_R1, uint16_t divider_R2, uint8_t i);
};



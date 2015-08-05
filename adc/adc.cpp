/*
    File:       adc.cpp
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

#include "adc.h"

void ADC10b::init() {
	// enable ADC, select ADC clock = F_CPU / 128 (i.e. 125 kHz)
	ADCSRA = (1<<ADEN | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0 );
	inited = true;
}

// Reads the ADC port specified by i
uint16_t ADC10b::read(uint8_t i) {
	if (!inited) {
		init();
		_delay_ms(1);
	}
	// no REFS0 and no REFS1: AREF, Internal Vref turned off
	// REFS0: AVCC with external capacitor at AREF pin
	// REFS0 + REFS1: Internal 2.56V Voltage Reference with external capacitor at AREF pin
	ADMUX = (1<<REFS0) | (1<<REFS1) | i;
	// ADSC: start one conversion
	ADCSRA |= 1 << ADSC;
	// wait for conversion
	while (ADCSRA & (1 << ADSC));
	// return 16bit result
	return ADCL | (ADCH << 8);
}

// return ADC port voltage computed against given VRef, with resistive divider
uint16_t ADC10b::readDivVoltage(float vref, uint16_t divider_R1, uint16_t divider_R2, uint8_t i) {
	return (uint16_t) (read(i) / 1024.0 * (divider_R1 + divider_R2) / divider_R2 * vref);
}

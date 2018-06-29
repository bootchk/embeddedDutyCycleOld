
#include "mcu.h"

#ifndef __MSP430__
#define __MSP430__
#define __MSP430FR4133__
#endif

#include "msp430.h"


void MCU::enterLPM4orLPM4_5(){

	/*
	 * Set certain bits in the mcu status register (SR)
	 *
	 * Alternatives:
	 * - LPM4 macro
	 * - __bis_SR_register(LPM4_bits);
	 */
	LPM4;
}

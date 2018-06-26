
#include "bridge.h"


/*
 * Choose mcu family and chip.
 * Must precede SPI.h
 * This configures pins for SPI interface to the default for the Launchpad i.e. the dev kit board.
 * I.E. configures a board.h file.
 *
 * A hack to get a clean compile in Eclipse IDE.
 * !!! Not necessarily correct, they just eliminate compiler errors.
 * Not needed if compiling in Energia IDE (where the Board Manager does magic.)
 * Proper paths also necessary.
 */
#define __MSP430__
#define __MSP430FR4133__ 1
// #define MSPEXP430FR2433LP 1

#include <SPI.h>

#include "../MCU/pinFunction.h"


/*
 * This implementation is the SPI choice for channel.
 * Uses Arduino/Energia library.
 */



/*
 * Routines for:
 * - quirks of RTC chip
 * - limitations of SPI library
 */
namespace {

// ABx8xx devices require clear upper bit of address, on read

unsigned char mangleReadAddress(Address address) {
	return (127 & (unsigned char) address);
}

// ABx8xx devices require set upper bit of address, on write
unsigned char mangleWriteAddress(Address address) {
	return (128 | (unsigned char) address);
}



/*
 * Transfer many bytes over SPI.
 *
 * The SPI.transfer is duplex or bidirectional: both read and write in the same transfer.
 * On write, we ignore what is read from slave.
 */
void readBuffer( unsigned char * bufferPtr, unsigned int size) {
	PinFunction::selectSPISlave();
	for(unsigned int i = 0; i < size; i++) {
		bufferPtr[i] = SPI.transfer(0);
	}
	PinFunction::deselectSPISlave();
}

void writeBuffer( unsigned char * bufferPtr, unsigned int size) {
	PinFunction::selectSPISlave();
	for(unsigned int i = 0; i < size; i++) {
		// ignore returned read
		(void) SPI.transfer(bufferPtr[i]);
	}
	PinFunction::deselectSPISlave();
}


} // namespace




/*
 * Three SPI pins are chosen by SPI library.
 *
 * Alternatively, the implementation could choose another SPI peripheral on the mcu,
 * using a different set of pins.
 *
 * See SPI::setModule() for choosing an alternate SPI peripheral.
 */
void Bridge::configureMcuSide() {
	// not require isSPIReady() since configuration is on the mcu side

	/*
	 * Configure a set of the mcu's GPIO pins for the SPI function i.e. a SPI module
	 * This only configures 3 of the 4 pins (not the Slave Select pin) ???
	 * TI Energia document doesn't say this configure MISO ???
	 */
	// TODO fix SPI pins
	/*
	 * we don't need to specify SS pin on Energia?
	 * Docs for begin() says is configures default SS pin in the SPI library.
	 * But code seems to indicate it doesn't.
	 * Although there exists an overloaded transfer(SSpin, value) method apparently not implemented on Energia.
	 */
	SPI.begin();

	/*
	 * Configure the mcu SPI peripheral with parameters of rtc chip's SPI
	 */
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV128);

	// Configure fourth pin: slave select
	PinFunction::configureSelectSPIPin();
}


void Bridge::unconfigureMcuSide() {
	SPI.end();
}




void Bridge::write(Address address, unsigned char value) {
	// require mcu SPI interface configured

	PinFunction::selectSPISlave();
	SPI.transfer(mangleWriteAddress(address));
	SPI.transfer( value);
	PinFunction::deselectSPISlave();
}



unsigned char Bridge::read(Address address) {
	// require mcu SPI interface configured

	unsigned char result;

	PinFunction::selectSPISlave();
	SPI.transfer(mangleReadAddress(address));
	result = SPI.transfer( 0 );
	PinFunction::deselectSPISlave();
	return result;
}



void Bridge::writeAlarm(RTCTime alarm) {

	PinFunction::selectSPISlave();
	SPI.transfer(mangleWriteAddress(Address::Alarm));
	writeBuffer((unsigned char*) &alarm, sizeof(alarm));
	PinFunction::deselectSPISlave();

	// assert alarm parameter is unchanged.
	// assert time was written to RTC
}



void Bridge::readTime(RTCTime* time) {

	PinFunction::selectSPISlave();
	SPI.transfer(mangleReadAddress(Address::Time));
	readBuffer((unsigned char*) time, sizeof(RTCTime));
	PinFunction::deselectSPISlave();
	// assert time buffer filled with read from rtc
}



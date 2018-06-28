
#include "bridge.h"

#include "../MCU/pinFunction.h"
#include "../SPI/serial.h"


/*
 * This implementation is the SPI choice for channel.
 * Uses Arduino/Energia library.
 */




namespace {

/*
 * Routines for protocol of RTC chip
 * Quirk: ABx8xx devices require clear upper bit of address, on read
 */

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
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
		bufferPtr[i] = Serial::transfer(0);
	}
}

void writeBuffer( unsigned char * bufferPtr, unsigned int size) {
	// require slave already selected
	for(unsigned int i = 0; i < size; i++) {
		// ignore returned read
		(void) Serial::transfer(bufferPtr[i]);
	}
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
	 * Enable serial device
	 * Configure a set of the mcu's GPIO pins for the serial peripheral/module
	 * e.g. a SPI module
	 */
	/*
	 * we don't need to specify SS pin on Energia?
	 * Docs for begin() says is configures default SS pin in the SPI library.
	 * But code seems to indicate it doesn't.
	 * Although there exists an overloaded transfer(SSpin, value) method apparently not implemented on Energia.
	 */
	Serial::begin();

	// Configure fourth pin: slave select
	PinFunction::configureSelectSPIPin();
}


void Bridge::unconfigureMcuSide() {
	Serial::end();
	// assert selectSPIPin is low power output
}




void Bridge::write(Address address, unsigned char value) {
	// require mcu Serial interface configured

	PinFunction::selectSPISlave();
	(void) Serial::transfer(mangleWriteAddress(address));
	(void) Serial::transfer( value);
	PinFunction::deselectSPISlave();
}



unsigned char Bridge::read(Address address) {
	// require mcu Serial interface configured

	unsigned char result;

	PinFunction::selectSPISlave();
	(void) Serial::transfer(mangleReadAddress(address));
	result = Serial::transfer( 0 );
	PinFunction::deselectSPISlave();
	return result;
}



void Bridge::writeAlarm(RTCTime alarm) {

	PinFunction::selectSPISlave();
	Serial::transfer(mangleWriteAddress(Address::Alarm));
	writeBuffer((unsigned char*) &alarm, sizeof(alarm));
	PinFunction::deselectSPISlave();

	// assert alarm parameter is unchanged.
	// assert time was written to RTC
}



void Bridge::readTime(RTCTime* time) {

	PinFunction::selectSPISlave();
	Serial::transfer(mangleReadAddress(Address::Time));
	readBuffer((unsigned char*) time, sizeof(RTCTime));
	PinFunction::deselectSPISlave();
	// assert time buffer filled with read from rtc
}



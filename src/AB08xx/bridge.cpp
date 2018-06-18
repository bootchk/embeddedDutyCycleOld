
#include "bridge.h"


#include <SPI.h>

// TODO
/*
 * we don't need to specify SS pin on Energia?
 * When we called begin(), we set SS pin in the SPI library.
 * We don't need to specify it again here.
 * Although there exists an overloaded transfer(SSpin, value) method
 */

/*
 * Implementation for SPI channel.
 * Uses Arduino/Energia library.
 */



/*
 * Routines for:
 * - quirks of RTC chip
 * - limitations of SPI library
 */
namespace {

// ABx8xx devices require clear upper bit of address, on read

unsigned char mangleReadAddress(unsigned char address) {
	return (127 & address);
}

// ABx8xx devices require set upper bit of address, on write
unsigned char mangleWriteAddress(unsigned char address) {
	return (128 | address);
}

/*
 * Transfer many bytes over SPI.
 *
 * The SPI.transfer is duplex or bidirectional: both read and write in the same transfer.
 * On write, we ignore what is read from slave.
 */
void readBuffer( unsigned char * bufferPtr, unsigned int size) {
	for(int i = 0; i < size; i++) {
		buf[i] = SPI.transfer(0);
	}
}

void writeBuffer( unsigned char * bufferPtr, unsigned int size) {
	for(int i = 0; i < size; i++) {
		// ignore returned read
		(void) SPI.transfer(buf[i]);
	}
}


} // namespace




/*
 * Four SPI pins are chosen by SPI library.
 *
 * Alternatively, the implementation could choose another SPI peripheral on the mcu,
 * using a different set of pins.
 *
 * See SPI::setModule() for choosing an alternate SPI peripheral.
 */
static void Bridge::configureMcuSide() {
	// not require isSPIReady() since configuration is on the mcu side

	/*
	 * Configure a set of the mcu's GPIO pins for the SPI function i.e. a SPI module
	 */
	// TODO the TI Energia document doesn't say this configure MISO??
	SPI::begin();

	/*
	 * Configure the mcu SPI peripheral with parameters of rtc chip's SPI
	 */
	SPI.setBitOrder(MSBFIRST);
	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV128);
}




void Bridge::write(Address address, unsigned char value) {
	// require mcu SPI interface configured



	SPI.transfer(pin, (128 | address));

	SPI.transfer( pin, value, SPI_LAST);
}



unsigned char Bridge::read(Address address, unsigned char value) {
	// require mcu SPI interface configured

	unsigned char result;

	SPI.transfer(mangleReadAddress(address));

	result = SPI.transfer( value, SPI_LAST);
	return result;
}



void Bridge::writeAlarm(RTCTime alarm) {
	SPI.transfer(mangleWriteAddress(Address::Alarm));

	writeBuffer((unsigned char*) &alarm, sizeof(time));

	// assert alarm parameter is unchanged.
	// assert time was written to RTC
}



void Bridge::readTime(RTCTime* time) {
	SPI.transfer(mangleReadAddress(Address::Time));

	readBuffer((unsigned char*) time, sizeof(time));

	// assert time buffer filled with read from rtc
}



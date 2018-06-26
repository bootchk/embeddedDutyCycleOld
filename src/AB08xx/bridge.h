
/*
 * Abstracts access to the RTC.
 *
 * A bridge across a channel.
 * Two different channels: SPI and I2C
 * Change channel by substituting a different implementation.
 *
 * Also hides any quirks of the device:
 * - the upper bit has special meaning.
 * - send address, then values
 */

#include "AB08xxRegisters.h"

#include "rtcTime.h"


class Bridge {
public:
	/*
	 * Configure mcu side of interface.
	 */
	static void configureMcuSide();

	/*
	 * Configure SPI pins to low power state.
	 *
	 * Energia docs for SPI lib say this does not change the GPIO configuration,
	 * only disable the SPI function on them (disables the driving device.)
	 * Since MOSI and SClk are already outputs, they are low power.
	 * MISO is an input, but driven by the RTC so it should be low power.
	 * The documents do not say this changes MISO from input to output.
	 *
	 * This does not affect the SlaveSelect pin.
	 * But it is an output, also low power.
	 */
	static void unconfigureMcuSide();


	static unsigned char read(Address);

	/*
	 * Write one byte to remote register at address
	 */
	static void write(Address, unsigned char value);

	/*
	 * Read Time register.
	 * Write Alarm register.
	 *
	 * For both, RTCTime is raw, i.e. still BCD encoded for AB08xx
	 */
	static void writeAlarm(RTCTime);
	static void readTime(RTCTime* time);
};

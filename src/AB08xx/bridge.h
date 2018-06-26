
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

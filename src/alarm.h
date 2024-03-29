

#include "alarmTypes.h"

/*
 * Provides an alarm from a remote RTC.
 *
 * Understands both sides of an interface from mcu to rtc using SPI bus and one other pin.
 *
 * Any other mcu device/pin configuration is the domain of the app.
 *
 *
 *
 * Algebra:
 *
 * waitSPIReadyOrReset must be called after power on and return true before other methods are called.
 *
 * configureMcuSPIInterface must precede configureRTC
 * configureMcuAlarmInterface can be done before or after (configureMcuSPIInterface, configureRTC)
 *
 * configureRTC and configureMcuAlarmInterface must precede setAlarm
 *
 * clearAlarm must be called after the alarm goes off and before setAlarm is called again
 *
 * There is no time setting function that must precede setAlarm()
 *
 * There is no method to cancel an alarm that has been set.
 * (Design assumes only one alarm.)
 *
 * Results of improper sequences:
 *
 * Calling setAlarm twice without an intervening clearAlarm ???
 *
 * Calling any method before isSPIReady returns true after POR???
 */
class Alarm {


	/*
	 * Returns true if RTC is out of reset and ready for SPI.
	 * IOW RTC Fout/nIRQ pin is high.
	 *
	 * The Fout/nIRQ pin has two purposes:
	 *  - is low when RTC is in reset and SPI is not ready
	 *  - is briefly low when the RTC is triggering an alarm.
	 *
	 * This should only be called when the mcu has been POR reset,
	 * to check that the RTC is out of reset.
	 *
	 * This is NOT general purpose.
	 * It depends on the RTC interrupt being a pulse,
	 * and the MCU interrupting on the rising edge
	 * (after Fout/nIRQ has returned to high state.)
	 * IOW it can not be safely called when the mcu has set an alarm
	 * but not gone to sleep, since the interrupt could come at any time,
	 * and the pulse lasts a finite duration,
	 * thus Fout/nIRQ could be low and this function return false
	 * when the RTC is really ready for SPI.
	 *
	 * No error returns.
	 */
	static bool isSPIReady();

public:

	/*
	 * Both might reset instead of return.
	 */
	/*
	 * Spin finite time waiting for rtc ready for SPI, i.e. out of reset.
	 *
	 * If the rtc did not reset,
	 * and since the same signal is used for an alarm interrupt and "SPI not ready"
	 * it is also possible that the rtc is interrupting on alarm,
	 * but the design uses a pulse interrupt, so the signal can only be low for a short time (1/4 second ?)
	 */
	static void waitSPIReadyOrReset();

	static void resetIfSPINotReady();

	/*
	 * Configure a set of mcu's GPIO pins for the mcu's SPI peripheral/module,
	 * and configure the mcu's SPI peripheral with parameters matching the rtc's SPI.
	 *
	 * No error return.
	 *
	 * The library does not support use of said GPIO pins for other purposes while the library is in use.
	 */
	static void configureMcuSPIInterface();


	/*
	 * Change SPI interface to a low power condition.
	 * When called, exists a set of in and out GPIO for SPI.
	 * Ensure all all are low power (inputs not floating.)
	 * Since RTC is expected to be alive and driving input pins, this might not do anything.
	 */
	static void unconfigureMcuSPIInterface();


	/*
	 * Configure one mcu GPIO pin for alarm interrupts from rtc.
	 */
	static void configureMcuAlarmInterface();

    /*
     * Over SPI, configure RTC clock mode and alarm interrupt.
     * Configuration is arbitrary (you could change it).
     */
	static void configureRTC();


private:
	/*
	 * Tell RTC to not assert alarm signal.
	 *
	 * Does NOT require Fout/nIRQ signal in any state.
	 * Fout/nIRQ might already be low (interrupt is a pulse.)
	 * Fout/nIRQ might still be high (still in the pulse.)
	 *
	 * Ensures RTC is not asserting signal Fout/nIRQ low because of an alarm.
	 *
	 * Returns false if error:
	 *  - SPI write error
	 *  - signal fails to become high
	 *
	 *  This does NOT require isSPIReady() == true.
	 *  See elsewhere.
	 *  The usual case is isSPIReady()==true when interrupt pulse is over and RTC is ready for SPI.
	 *  (Especially when interrupt pulse is low-to-high edge triggered.)
	 *  The unusual and rare event is that isSPIReady()==false because of RTC reset
	 *  (during falling Vcc, when mcu is still alive but RTC is resetting.)
	 *
	 *  Case 1:
	 *  If RTC is still in the interrupt pulse,
	 *  Fout/nIRQ is still low and isSPIReady() would return false, even though the RTC is ready for SPI.
	 *  This method never the less attempts to clear the interrupt flag on the RTC.
	 *  According to the RTC datasheet, that should immediately end the interrupt pulse (taking Fout/nIRQ high).
	 *  If there is an SPI error or for any other failure,
	 *  and Fout/nIRQ does not go low, this method returns false.
	 *
	 *  Case 2:
	 *  If RTC is in reset, Fout/nIRQ will be low and SPI not ready.
	 *  isSPIReady() would return false.
	 *  This method proceeds anyway to attempt SPI communication.
	 *  That only succeeds if the method reads from SPI what was written AND Fout/nIRQ goes high.
	 *  When the RTC comes out of reset too late in this method,
	 *  the SPI communication fails and and this method fails
	 *  even though Fout/nIRQ is high (because that is the reset state for it.)
	 *
	 *  If the RTC comes out of reset early enough,
	 *  the SPI communication succeeds
	 *  and this method succeds.
	 */
	static bool clearAlarmOnRTC();


public:
	/*
	 * Might reset instead of returning.
	 * RTC is remote device that may fail.
	 */
	static void clearAlarmOnRTCOrReset();


	/*
	 * Clear alarm interrupt flag on MCU side of signal.
	 */
	static void clearAlarmOnMCU();

	/*
	 * Set alarm to go off duration from now.
	 *
	 * Returns false  when:
	 * - duration too short
	 * - library state errors (see algebra)
	 */
	static bool setAlarm(Duration);


private:
	/*
	 * Is logical state of signal high?
	 * Does not mean the mcu interrupt flag is not set.
	 * The interrupt is usually generated on an upward edge of downward pulse on signal.
	 * The interrupt exists after the signal goes high again.
	 *
	 * The signal is on a net of two pins:
	 *  - RTC Fout/nIRQ pin
	 *  - some GPIO pin of the mcu
	 */
	static bool isAlarmInterruptSignalHigh();

};

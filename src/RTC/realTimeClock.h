
#include "../alarmTypes.h"	// Duration


/*
 * Abstracts RTC methods.
 *
 * Implementation is via writes and reads to registers of external device rtc.
 * This hides:
 *  - registers and values
 *  - combinations of values required for certain actions
 *
 * Writes and reads are via SPI.
 * TODO make the writer/reader layer independent of SPI/I2C using other than inheritance in current AB08xx lib
 */

class RTC {
public:
	/*
	 * Clear interrupt flag that generated alarm interrupt.
	 * The flag must be cleared before another alarm can be set.
	 * Since the interrupt is downward pulse (1/64 second)  from rtc,
	 * and interrupt on the mcu is rising edge,
	 * Fout/nIRQ should already be high.
	 *
	 * No error return.
	 * Caller must check that Fout/nIRQ is high.
	 */
	static void clearIRQInterrupt();


	/*
	 * TODO
	 */
	static bool setAlarm(Duration);



	/*
	 * Configure RTC to use an oscillator that is both precise and low power.
	 * See data sheet for details and other modes.
	 * If you don't call this, RTC defaults at reset to XTAL oscillator mode.
	 */
	static void configureRCCalibratedOscillatorMode();

	/*
	 * Configure RTC so only alarm interrupts are on pin.
	 */
	static void configureAlarmInterruptToFoutnIRQPin();



private:
	static void selectOscModeRCCalibratedWithAutocalibrationPeriod();
	static void enableAutocalibrationFilter();
	static void enablePulseInterruptForAlarm();
	static void connectFoutnIRQPinToAlarmSignal();
};

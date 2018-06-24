
#include "alarmLib.h"


#include "RTC/realTimeClock.h"  // Avoid clash with rtc.h"
#include "AB08xx/bridge.h"	//  hides SPI
#include "pinfunction.h"			// hides GPIO functions




/*
 * RTC signals SPI not ready (during reset) by asserting rtc:Fout/nIRQ pin low.
 * !!! Same pin as for the Alarm signal.
 */
bool AlarmLib::isSPIReady() {
	return (AlarmLib::isAlarmInterruptSignalHigh());
}



bool AlarmLib::clearAlarmOnRTC() {
	// Tell RTC to end interrupt pulse (signal to high) if not already so

	RTC::clearIRQInterrupt();
	if (!isAlarmInterruptSignalHigh())
		return false;	// SPI write failed

	return (AlarmLib::isAlarmInterruptSignalHigh());

	// ensure RTC interrupt flag is clear
	// ensure interrupt signal net is high
}


void AlarmLib::clearAlarmOnMCU() {
	PinFunction::clearAlarmInterruptOnPin();
}


bool AlarmLib::isAlarmInterruptSignalHigh() {
	// requires pin configured as input

	/*
	 * GPIO_getInputPinValue returns a unsigned byte result for pin mask byte argument.
	 * Non-zero result means AlarmSignalPin is high.
	 */
	return PinFunction::isAlarmPinHigh();
}



void AlarmLib::configureMcuSPIInterface(){
	Bridge::configureMcuSide();
}


void AlarmLib::configureMcuAlarmInterface() {
	/*
	 * Pin is high when no interrupt.
	 * RTC pulses low on interrupt.
	 * Pulse width is relatively long (1/4 second)
	 * Use trailing edge, low-to-high
	 */
	PinFunction::configureAlarmPinPullupLoToHiInterrupt();

}



void AlarmLib::configureRTC() {
	// require configureMcuSPIInterface

	// Order of configuration not important.

	RTC::configureRCCalibratedOscillatorMode();

	RTC::configureAlarmInterruptToFoutnIRQPin();
}


/*
 * Must be bulletproof since is alarm is failed to set, may sleep forever.
 */
bool AlarmLib::setAlarm(Duration duration) {
	bool result = false;

	// delegate to RTC
	result = RTC::setAlarm(duration);

	// ensure alarm is set
	return result;
}


#include "alarmLib.h"

# mcu hal layer e.g. MSPWare DriverLib
#include <gpio.h>	// depends on msp430.h

# private library
#include "rtc.h"

// layer that hides SPI
#include "AB08xx/bridge.h"




// rename from hal namespace to alarmLib namespace
#define AlarmSignalPort GPIO_PORT_P1
#define AlarmSignalPin  GPIO_PIN6




/*
 * RTC signals SPI not ready (during reset) by asserting rtc:Fout/nIRQ pin low.
 * !!! Same pin as for the Alarm signal.
 */
bool AlarmLib::isSPIReady() {
	return (AlarmLib::isAlarmInterruptSignalHigh());
}



bool AlarmLib::clearAlarmOnRTC() {
	// Tell RTC to end interrupt pulse (signal to high) if not already so
	if (! RTC::clearIRQInterrupt() )
		return false;	// SPI write failed

	return (AlarmLib::isAlarmInterruptSignalHigh());

	// ensure RTC interrupt flag is clear
	// ensure interrupt signal net is high
}


void AlarmLib::clearAlarmInterruptOnMcu() {
	GPIO_clearInterrupt(AlarmSignalPort, AlarmSignalPin);
}


bool AlarmLib::isAlarmInterruptSignalHigh() {
	// requires pin configured as input

	/*
	 * GPIO_getInputPinValue returns a unsigned byte result for pin mask byte argument.
	 * Non-zero result means AlarmSignalPin is high.
	 */
	return (GPIO_getInputPinValue(AlarmSignalPort, AlarmSignalPin) != 0);
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
	GPIO_setAsInputPinWithPullUpResistor(AlarmSignalPort, AlarmSignalPin);
	GPIO_enableInterrupt(AlarmSignalPort, AlarmSignalPin);
	GPIO_selectInterruptEdge(AlarmSignalPort, AlarmSignalPin, GPIO_LOW_TO_HIGH_TRANSITION);
}



void AlarmLib::configureRTC() {
	// require configureMcuSPIInterface

	// Order of configuration not important.

	RTC::configureRCCalibratedOscillatorMode();

	RTC::configureAlarmInterruptToFoutnIRQPin();
}


bool AlarmLib::setAlarm(Duration duration) {
	bool result = false;

	// delegate to RTC
	result = RTC::setAlarm(duration);

	// ensure alarm is set
	return result;
}

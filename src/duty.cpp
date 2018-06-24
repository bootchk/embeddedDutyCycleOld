
#include "alarmLib.h"
#include "duty.h"








/*
 * The mcu has seen a power on reset event (Vcc rising).
 * The mcu was not in LPM sleep previously (a hard reset.)
 * The design intends that the rtc also suffered a reset.
 */
void Duty::onPowerOnReset() {

	/*
	 * Spin finite time waiting for rtc ready for SPI, i.e. out of reset.
	 *
	 * If the rtc did not reset,
	 * and since the same signal is used for an alarm interrupt and "SPI not ready"
	 * it is also possible that the rtc is interrupting on alarm,
	 * but the design uses a pulse interrupt, so the signal can only be low for a short time (1/64 second TODO)
	 */
	int i = 0;

	while ( ! AlarmLib::isSPIReady() ) {
		i++;
		if (i > 100) {
			// TODO sw reset
		}
	}
	// assert alarm interrupt signal is high
	// mcu pin resets to an input, but without interrupt enabled

	// Assume rtc was reset also

	// Must precede use of SPI to configure rtc
	AlarmLib::configureMcuSPIInterface();

	AlarmLib::configureMcuAlarmInterface();

	AlarmLib::configureRTC();

	// ensure AlarmLib is ready for setAlarm()

	// caller typically calls setAlarm() and then sleeps.
}


void Duty::onWakeForAlarm() {
	/*
	 * Conditions:
	 *
	 * mcu was reset (LPM4.5 ends with reset, but not a POR).
	 * rtc has not been POR reset.
	 *
	 * If mcu was reset, caller configured GPIO for AlarmLib's alarm pin,
	 * but not for AlarmLib's SPI pins.
	 * The design is weird, see LPM4.5.
	 *
	 * rtc:Fout/nIRQ pin was pulsed low but now is high.
	 * The alarm interrupt is rising edge.
	 */

	// TODO Configure mcu spi pins etc.

	/*
	 * Fail means the system is in invalid state (mcu not POR, but rtc is POR)
	 */
	 if (!AlarmLib::isSPIReady()) {
		 // TODO reset
	 }

	/*
	 * Clear alarm on rtc side.
	 *
	 * Reset mcu since
	 * Fail means RTC may be continuing to generate interrupt signal on Fout/nIRQ
	 */
	if (!AlarmLib::clearAlarmOnRTC())  {
		// TODO sw reset
	}

	AlarmLib::clearAlarmOnMCU();

	/*
	 * Assert interrupts are cleared and no more will come, yet.
	 *
	 * Continuation typically is to act, then sleep mcu.
	 */
}


bool Duty::setAlarm(unsigned int duration) {
	return AlarmLib::setAlarm(duration);
}

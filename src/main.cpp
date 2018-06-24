
#include "duty.h"
#include "mcuSleep.h"


#define DURATION 100



int main(void) {

	/*
	 * Reset has occurred in one of two ways:
	 * - from sleep
	 * - from power on
	 */

	Duty::restoreMCUToPresleepConfiguration();

	/*
	 * Dispatch on reason for wake.
	 */
	if (MCUSleep::isResetAWakeFromSleep()) {

		MCUSleep::unlockMCUFromSleep();
		// Interrupt is service now.
		Duty::onWakeForAlarm();
		// TODO app should act here
	}
	else {
		MCUSleep::unlockMCUFromSleep();
		Duty::onPowerOnReset();
	}

	// TODO DURATION should depend on app state.
	/*
	 * Fail means system might sleep forever, so only adequate response is reset mcu
	 */
	if (!Duty::setAlarm(DURATION)) {
		// TODO sw reset
	}

	/*
	 * Assert mcu is in presleep condition.
	 * E.G. GPIO is not configured for SPI.
	 *
	 * Assert some interrupt will come (E.G. a Duty Alarm)
	 * else we would sleep forever.
	 */
	/*
	 * Does not return.  Continuation is a reset.
	 * We don't need an infinite loop coded here, the loop is via interrupt and reset back to main()
	 */
	MCUSleep::enterLowestPowerSleep();
	// Never get here...
}

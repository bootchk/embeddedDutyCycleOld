
#include "duty.h"
#include "mcuSleep.h"

#include "MCU/powerMgtModule.h"   // stopWatchdog


#define DURATION 100



int main(void) {

	/*
	 * Reset has occurred:
	 * - from sleep
	 * - from power on or other reasons (Vcc faults, bus faults?)
	 */

	PMM::stopWatchdog();

	Duty::restoreMCUToPresleepConfiguration();

	/*
	 * Dispatch on reason for wake.
	 */
	if (MCUSleep::isResetAWakeFromSleep()) {
		MCUSleep::clearIsResetAWakeFromSleep();

		MCUSleep::unlockMCUFromSleep();
		// Interrupt is serviced now, if presleep configuration enables interrupts
		Duty::onWakeForAlarm();
		// TODO app should act using persistent state (in FRAM)
	}
	else {	// power on reset
		// Reset clears lock bit.  No need for: MCUSleep::unlockMCUFromSleep();

		Duty::onPowerOnReset();

		// TODO app should initialize state (FRAM is initialized at load time, not in resetHandler
	}

	// TODO DURATION should depend on app state.
	/*
	 * Resets if fail to set alarm
	 */
	Duty::setAlarmOrReset(DURATION);

	/*
	 * Assert mcu is in presleep condition.
	 * E.G. GPIO is not configured for SPI
	 * TODO
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

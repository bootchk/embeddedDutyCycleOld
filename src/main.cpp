
#include "duty.h"
#include "mcuSleep.h"
#include "app.h"

#include "MCU/powerMgtModule.h"   // stopWatchdog


#define DURATION 100



int main(void) {

	/*
	 * Reset has occurred:
	 * - from sleep
	 * - from power on or other reasons (Vcc faults, bus faults?)
	 */

	PMM::stopWatchdog();

	/*
	 * Establish base to be overridden.
	 * This is not effective immediately in all cases, when GPIO is locked (on wake from alarm.)
	 */
    MCUSleep::configureAllPinsLowPower();

	/*
	 * Dispatch on reason for wake.
	 */
	if (MCUSleep::isResetAWakeFromSleep()) {
		MCUSleep::clearIsResetAWakeFromSleep();

		/*
		 * GPIO config registers were reset (but GPIO state is held.)
		 * Preconfigure as it was before sleep.
		 * Effective upon unlockMCUFromSleep()
		 */
		Duty::restoreMCUToPresleepConfiguration();

		MCUSleep::unlockMCUFromSleep();

		// Interrupt is serviced now, if presleep configuration enables interrupts

		Duty::onWakeForAlarm();
		App::onWakeForAlarm();
	}
	else {	// power on reset
		// Reset clears lock bit.  No need for: MCUSleep::unlockMCUFromSleep();

		/*
		 * GPIO configuration is reset.
		 * Configure Duty and App.
		 * Effective immediately, since not locked.
		 */
		Duty::onPowerOnReset();
		App::onPowerOnReset();
	}

	// Resets if fail to set alarm
	Duty::setAlarmOrReset(App::durationOfSleep());
	Duty::lowerMCUToPresleepConfiguration();

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

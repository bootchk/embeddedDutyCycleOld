
#include "duty.h"
#include "mcuSleep.h"
#include "app.h"

#include "MCU/powerMgtModule.h"   // stopWatchdog

#include <msp430.h>   // PORT1_VECTOR


/*
 * ISR for RTC alarm interrupt.
 *
 * Alarm pin is P1.3, so need ISR for Port 1
 *
 * It is possible to eliminate this if you don't enable interrupt after sleep before unlocking.
 * Then the ISR is not called, even though interrupt occurred.
 *
 * Here, the ISR just clears interrupt flag, so no infinite interrupt loop.
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	Duty::clearAlarmOnMCU();
}



int main(void) {

	/*
	 * Reset has occurred:
	 * - from sleep
	 * - OR from power on or other reasons (Vcc faults, bus faults?)
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

		/*
		 * Trap to allow debugger to synch
		 */
		bool isTrapped;
		isTrapped = true;
		while(isTrapped);

		MCUSleep::clearIsResetAWakeFromSleep();

		/*
		 * GPIO config registers were reset (but GPIO state is held.)
		 * Preconfigure as it was before sleep.
		 * Effective upon unlockMCUFromSleep()
		 */
		Duty::restoreMCUToPresleepConfiguration();
		App::configureSleepingGPIO();

		MCUSleep::unlockMCUFromSleep();

		// Interrupt is serviced now, if presleep configuration enables interrupts

		Duty::onWakeForAlarm();
		// assert alarm interrupt is cleared and Duty is ready for setAlarm
		App::onWakeForAlarm();
		// assert app done with useful work
		// assert GPIO in sleeping configuration
	}
	else {	// power on reset or other reasons
		// POR reset clears lock bit but other reasons may not.  Requires unlocked, so safer to always unlock.
		MCUSleep::unlockMCUFromSleep();

		/*
		 * GPIO configuration is reset.
		 * Configure Duty and App.
		 * Effective immediately, since GPIO not locked.
		 */
		Duty::onPowerOnReset();
		// assert Duty is ready for setAlarm
		App::onPowerOnReset();
		// assert app in initial state and GPIO in sleeping configuration
	}

	// Assert app is done with its useful work, or is in intial state

	// Assert app has unconfigured any devices used ephemerally in its useful work
	// Some GPIO pins that app persists during sleep may still be in use (e.g. an LED)

	// Resets if fail to set alarm
	Duty::setAlarmOrReset(App::durationOfSleep());

	Duty::lowerMCUToPresleepConfiguration();

	/*
	 * Assert mcu is in presleep condition E.G. GPIO is not configured for Duty using SPI
	 *
	 * Assert some interrupt will come (E.G. a Duty Alarm) else we would sleep forever.
	 */
	/*
	 * Does not return.  Continuation is a reset.
	 * We don't need an infinite loop coded here, the loop is via interrupt and reset back to main()
	 */
	MCUSleep::enterLowestPowerSleep();
	// Never get here...
}


/*
 * Understands how to sleep mcu.
 *
 * For a design where:
 * - GPIO is locked during sleep but responsive to waking interrupts
 * - wake from sleep is a reset
 *
 * Implementation is for TI's LPM4.5 on MSP430
 */

class MCUSleep {
public:

	/*
	 * Sleep mcu.  Only wake will be a reset of some sort, usually from an interrupt.
	 */
	static void enterLowestPowerSleep();

	/*
	 * Must follow isResetAWakeFromSleep
	 * Once unlocked, you can't tell whether reset was from sleep.
	 */
	static void unlockMCUFromSleep();

	/*
	 * Called when mcu has been reset.
	 * True if sleeping before this reset.
	 * False if not sleeping, and therefore a power on or software reset or other reset.
	 */
	static bool isResetAWakeFromSleep();
};

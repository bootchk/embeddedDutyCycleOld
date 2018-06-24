

/*
 * Duty cycle the mcu (sleep, wake on alarm from rtc.)
 *
 * Layer below main.c
 * Hides implementation of alarms.
 *
 * Specialized for MSP430 LPM4.5.
 *
 * The library sw resets the mcu when it detects improper state.
 * In the implementation assert(false) software resets mcu.
 * asserts are NOT optional (should not be disabled with NDEBUG).
 *
 * Improper state could happen during falling Vcc, if the RTC is in reset before the mcu.
 * The design does not depend on any particular timing of mcu and rtc entering reset on falling Vcc.
 */


class Duty {
public:

	static void restoreMCUToPresleepConfiguration();


	/*
	 * TODO move this Configures mcu GPIO pins for AlarmLib
	 *
	 * Called infrequently.
	 * The system is intended to avoid power on reset caused by exhausting power.
	 *
	 * Exceptions:
	 * sw resets the mcu if rtc is non-communicative (times out or other error).
	 * Since this is called on reset, and might sw reset the mcu,
	 * the mcu could continually reset while rtc is not functioning.
	 */
	static void onPowerOnReset();

	/*
	 * Called on wake for alarm interrupt,
	 * which is a reset (for TI LPM4.5, called BOR reset)
	 *
	 * Called frequently as part of duty cycling.
	 */
	static void onWakeForAlarm();

	/*
	 * Delegated, see Alarm class.
	 *
	 * Ensures that mcu is in presleep configuration.
	 * Caller should follow immediately with sleep.
	 */
	static bool setAlarm(unsigned int);
};

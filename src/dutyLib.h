

/*
 * Duty cycle the mcu (sleep, wake on alarm from rtc.)
 *
 * Layer between main.c and alarmLib
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
	/*
	 * TODO move this Configures mcu GPIO pins for AlarmLib
	 *
	 * Called on power on reset.
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
	 */
	static void onWakeForAlarm();
};

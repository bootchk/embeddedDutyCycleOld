
/*
 * Facade to Power Management Module
 */
class PMM {
public:
	static void configureOff();

	static void triggerSWBORReset();

	static void unlockLPM5();

	/*
	 * A flag set by HW, checkable, and must be cleared.
	 */
	static bool isResetAWakeFromSleep();
	static void clearIsResetAWakeFromSleep();
};

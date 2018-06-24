
/*
 * Abstracts functions of GPIO pins.
 *
 * Two pins.
 *
 * Three other pins are used by SPI.
 */
class Pins {
public:
	static void selectSPISlave();
	static void deselectSPISlave();

	static void configureAlarmPinPullupLoToHiInterrupt();
	static void clearAlarmInterruptOnPin();
	static bool isAlarmPinHigh();
};

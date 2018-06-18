

/*
 * Safe, by reading what was written, and comparing.
 *
 * The SPI bus is on the PCB.
 * It COULD be subject to bit errors.
 * Bit error rate (BER) is extremely low.
 *
 * Errors arise:
 *
 * - Noise: BER depends on SPI speed and other physical factors of the PCB.
 * - Underlying SPI library might not be interrupt safe.
 * - RTC entering reset during the call
 *
 * Methods return false if fail to read what was written.
 */
class SafeSPI {
public:


	static bool clearBit();

	/*
	 * Clear a register by writing a 0 byte to it
	 */
	static void clearRegister(unsigned char);
};

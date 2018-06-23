
/*
 * Knows how to convert time formats.
 *
 * !!! With many subtle omissions.
 *
 * Goal here is to use Unix std implementations when possible.
 */

// Unix std library
// ARM gcc #include <sys/features.h>
// #include <features.h>

#include <ctime>	// C time.h

// Depends on type defined by RTC chip
#include "../AB08xx/rtcTime.h"



class TimeConverter {
public:
	/*
	 * Convert BCD encoded calendar into int encoded calendar.
	 * and vice versa
	 */
	static struct tm convertRTCTimeToCalendarTime(RTCTime);
	static RTCTime convertCalendarTimeToRTCTime(struct tm);

	/*
	 * Convert calendar time to epoch time and vice versa
	 */
	static time_t convertCalendarTimeToEpochTime(struct tm&);
	static struct tm convertEpochTimeToCalendarTime(time_t);
};


/*
 * Part of implementation of class RTC.
 * The part dealing with alarm setting.
 * Which is sufficiently complex and unrelated that it is in a separate file.
 */

#include "rtc.h"

#include "../AB08xx/bridge.h"
#include "timeConverter.h"

/*
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type time_t (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
void RTC::setAlarm(Duration duration) {

	// check preconditions

	RTCTime now = Bridge::readTime();

	// 2 step conversion from RTCTime to epoch time
	struct tm calendarTime = TimeConverter::convertRTCTimeToCalendarTime(now);
	time_t nowTime = TimeConverter::convertCalendarTimeToEpochTime(calendarTime);

	// calculate time of alarm
	time_t alarmEpochTime = nowTime + duration;

	// Reverse conversions
	struct tm alarmCalendarTime = TimeConverter::convertEpochTimeToCalendarTime(alarmEpochTime);
	RTCTime alarmRTCTime = TimeConverter::convertCalendarTimeToRTCTime(alarmCalendarTime);

	Bridge::writeAlarm(alarmRTCTime);
	// ensure alarm is set
}

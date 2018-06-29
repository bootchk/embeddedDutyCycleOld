
/*
 * Part of implementation of class RTC.
 * The part dealing with alarm setting.
 * Which is sufficiently complex and unrelated that it is in a separate file.
 */

#include "../AB08xx/bridge.h"
#include "realTimeClock.h"
#include "timeConverter.h"

/*
 * Implementation is largely converting type (RTCTime) that RTC delivers
 * to type EpochTime (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
bool RTC::setAlarm(Duration duration) {

	// TODO later, check preconditions for setting alarm
	// duration is great enough

	RTCTime now;
	Bridge::readTime(&now);

	// 2 step conversion from RTCTime to epoch time
	CalendarTime calendarTime = TimeConverter::convertRTCTimeToCalendarTime(now);
	EpochTime nowTime = TimeConverter::convertCalendarTimeToEpochTime(calendarTime);

	// calculate time of alarm
	EpochTime alarmEpochTime = nowTime + duration;

	// Reverse conversions
	CalendarTime alarmCalendarTime = TimeConverter::convertEpochTimeToCalendarTime(alarmEpochTime);
	RTCTime alarmRTCTime = TimeConverter::convertCalendarTimeToRTCTime(alarmCalendarTime);

	Bridge::writeAlarm(alarmRTCTime);

	// ensure alarm is set properly by reading it and comparing
	// TODO later, verify alarm is properly set,
	return true;
}

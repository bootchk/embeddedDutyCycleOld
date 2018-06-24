
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
 * to type time_t (seconds since epoch) so we can use simple math to add Duration
 * then reverse conversion back to the type (RTCTime) that RTC expects.
 */
bool RTC::setAlarm(Duration duration) {

	// check preconditions

	RTCTime now;
	Bridge::readTime(&now);

	// 2 step conversion from RTCTime to epoch time
	TimeElements calendarTime = TimeConverter::convertRTCTimeToCalendarTime(now);
	time_t nowTime = TimeConverter::convertCalendarTimeToEpochTime(calendarTime);

	// calculate time of alarm
	time_t alarmEpochTime = nowTime + duration;

	// Reverse conversions
	TimeElements alarmCalendarTime = TimeConverter::convertEpochTimeToCalendarTime(alarmEpochTime);
	RTCTime alarmRTCTime = TimeConverter::convertCalendarTimeToRTCTime(alarmCalendarTime);

	Bridge::writeAlarm(alarmRTCTime);

	// ensure alarm is set properly by reading it and comparing
	// TODO
	return false;
}


#include "timeConverter.h"



namespace {

/*
 * Each nibble is a decimal digit.
 */
uint8_t bcd2bin(uint8_t value)
{
	return (value & 0x0F) + ((value >> 4) * 10);
}

uint8_t bin2bcd(uint8_t value)
{
	return ((value / 10) << 4) + value % 10;
}

/*
// NOT USED
int yearOfCenturyToTm(int yearOfCentury) {
return 0;
}
*/

} // namespace



/*
 * Conversion offsets the year from different references.
 * Coming from RTC chip, year is offset from century.
 * Epoch time is offset from 1970.
 * We don't really care (as long as we omit conversion in both directions )
 * since no human is reading the results.
 *
 * Omit hundredths, not in Unix type tm
 * calendarTime.Hundreths = bcd2bin(buf.hundreths);
 *
 * !!! Omit superfluous fields:
 * tm_wday
 * tm_yday
 * tm_isdst
 *
 * We are not using the calendar for printing, the typical use for these fields.
 * These fields are not used in reverse conversion (see makeTime() source code)
 */

TimeElements TimeConverter::convertRTCTimeToCalendarTime(RTCTime& rtcTime) {
	TimeElements calendarTime;

	calendarTime.Year = bcd2bin(rtcTime.YearOfCentury); // Not used: CalendarYrToTm(); // /*+ (buf.weekdays.GP * 100)*/) + 2000;
	calendarTime.Month = bcd2bin(rtcTime.Month);
	calendarTime.Day = bcd2bin(rtcTime.DayOfMonth);
	calendarTime.Hour = bcd2bin(rtcTime.Hour24);
	calendarTime.Minute = bcd2bin(rtcTime.Minute);
	calendarTime.Second = bcd2bin(rtcTime.Second);

	return calendarTime;
}


RTCTime TimeConverter::convertCalendarTimeToRTCTime(TimeElements& calendarTime) {
	RTCTime rtcTime;

	rtcTime.YearOfCentury = bin2bcd(calendarTime.Year);
	rtcTime.Month         = bin2bcd(calendarTime.Month);
	rtcTime.DayOfMonth    = bin2bcd(calendarTime.Day);
	rtcTime.Hour24        = bin2bcd(calendarTime.Hour);
	rtcTime.Minute        = bin2bcd(calendarTime.Minute);
	rtcTime.Second        = bin2bcd(calendarTime.Second);

	return rtcTime;
}





time_t TimeConverter::convertCalendarTimeToEpochTime(TimeElements& calendarTime) {
	time_t epochTime  = 0 ;
	// Equivalent to Unix mktime()
	epochTime = makeTime( calendarTime) ;
	return epochTime;

}

TimeElements  TimeConverter::convertEpochTimeToCalendarTime( time_t epochTime) {
	TimeElements calendarTime ;
	// Equivalent to Unix localtime()
	breakTime( epochTime, calendarTime ) ;
	return calendarTime;

}

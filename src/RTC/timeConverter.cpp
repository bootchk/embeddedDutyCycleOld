
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
// TODO Tm years is since 1900
int yearOfCenturyToTm(int yearOfCentury) {
return 0;
}
*/

} // namespace



/*
 * Conversion offsets the year from different references.
 * Coming from RTC chip, year is offset from century.
 * On conversion to epoch time, it is offset from 1970.
 */

TimeElements TimeConverter::convertRTCTimeToCalendarTime(RTCTime& rtcTime) {
	TimeElements calendarTime;

	/*
	 * Field names Unix std: tm_year, etc.
	 */
	calendarTime.Year = bcd2bin(rtcTime.YearOfCentury); // CalendarYrToTm(); // /*+ (buf.weekdays.GP * 100)*/) + 2000;
	calendarTime.Month = bcd2bin(rtcTime.Month);
	calendarTime.Day = bcd2bin(rtcTime.DayOfMonth);
	calendarTime.Hour = bcd2bin(rtcTime.Hour24);
	calendarTime.Minute = bcd2bin(rtcTime.Minute);
	calendarTime.Second = bcd2bin(rtcTime.Second);

	// Omit hundredths, not in Unix type tm
	// calendarTime.Hundreths = bcd2bin(buf.hundreths);

	// TODO is this valid?
	/*
	 * Omit superflous fields of Unix type tm
	 * We are not using the calendar for printing, the typical use for these fields.
	 * I assume these fields are not used in reverse conversion.
	 *
	 * Omit:
	 * tm_wday
	 * tm_yday
	 * tm_isdst
	 */

	return calendarTime;
}


time_t TimeConverter::convertCalendarTimeToEpochTime(TimeElements& calendarTime) {
	time_t epochTime  = 0 ;
	// Equivalent to Unix mktime()
	epochTime = makeTime( calendarTime) ;
	return epochTime;

}

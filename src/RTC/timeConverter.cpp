
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

} // namespace




struct tm TimeConverter::convertRTCTimeToCalendarTime(RTCTime rtcTime) {
	struct tm calendarTime;

	/*
	 * Field names Unix std
	 */
	calendarTime.tm_year = CalendarYrToTm(bcd2bin(buf.years); // /*+ (buf.weekdays.GP * 100)*/) + 2000;
	calendarTime.tm_mon = bcd2bin(rtcTime.Month);
	calendarTime.tm_mday = bcd2bin(rtcTime.DayOfMonth);
	calendarTime.tm_hour = bcd2bin(rtcTime.Hour24);
	calendarTime.tm_min = bcd2bin(rtcTime.Minute);
	calendarTime.tm_sec = bcd2bin(rtcTime.Second);

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

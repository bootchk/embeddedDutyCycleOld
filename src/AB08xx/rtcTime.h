
#pragma once


/*
 * Struct holding part of the time kept by the RTC.
 * Both the time and alarm registers have this format.
 *
 * Omit weekday.
 *
 * Is encoded BCD.
 * Year in range [0, 99]
 */
struct RTCTime {
	unsigned char Hundredth;
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour24;	// 24 hour mode,range [0, 23]
	unsigned char DayOfMonth;
	unsigned char Month;
	unsigned char YearOfCentury;
}  ;


/*
 * Broken-down calendar time.
 *
 * Named same as Unix struct.
 *
 * tm
 *
 * conversion functions are similar to Unix mktime and localtime,
 * except that they are not relative to the "Unix epoch"
 * but relative to epoch "Mon 00:00:00 Jan. 1 1900"
 */
/*typedef struct {
	unsigned char Hundredth;
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;	// 24 hour mode,range [0, 23]
	unsigned char DayOfMonth;
	unsigned char Month;
	unsigned char YearOfCentury;
} RTCTime ;
*/




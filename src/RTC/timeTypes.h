
#pragma once

/*
 *
 */

typedef struct  {
  unsigned char Second;
  unsigned char Minute;
  unsigned char Hour;

  unsigned char Day;
  unsigned char Month;
  unsigned char Year;   // offset from 1970;
} CalendarTime;


// unsigned char Wday;   // day of week, sunday is day 1


typedef unsigned long EpochTime;

#include <debug.h>
#include "clock.h"
#include "rtttl.h"

///////////////////////////////////////////////////////////////////////////////
const uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t leapFeb = 29;

///////////////////////////////////////////////////////////////////////////////
_clock_t clock = {14, 12, 59, 30, 1, 25};
_alarm_t alarms[ALARMS] = {
  {8, 0, 91}
};

/**
 * Get the Month Days
 */
uint8_t clock_monthDays() {
  if ((clock.month == 2 /* February */) && ((clock.year == 0) || ((clock.year % 4) == 0))) {
    return leapFeb;
  } else {
    return monthDays[clock.month];
  }
}

/**
 * Check alarms
 */
void checkAlarms() {
  for (uint8_t pos = 0; pos < ALARMS; pos++) {
    if (alarms[pos].melody > 0) {
      if (alarms[pos].hour == clock.hour && alarms[pos].minute == clock.minute) {
        rtttl_play(alarms[pos].melody);
      }
    }
  }
}

/**
 * Increment seconds
 * 
 * @return uint8_t day
 */
void clock_tick() {
  clock.second++;
  if (clock.second > 59) {
    clock.second = 0;

    clock.minute++;
    if (clock.minute > 59) {
      clock.minute = 0;

      clock.hour++;
      if (clock.hour > 23) {
        clock.hour = 0;

        clock.day++;
        if (clock.day > clock_monthDays()) {
          clock.day = 0;

          clock.month++;
          if (clock.month > 12) {
            clock.month = 0;

            clock.year++;
            if (clock.year > 99) {
              clock.year = 0;
            }
          }
        }
      }
    }
  }
}

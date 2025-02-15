#include <debug.h>
#include "clock.h"

const uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t leapFeb = 29;

/**
 * Get the Month Days
 * 
 * @return uint8_t 
 */
uint8_t calculateMonthDays(_clock_t *clock) {
  if ((clock->month == 2 /* February */) && ((clock->year == 0) || ((clock->year % 4) == 0))) {
    return leapFeb;
  } else {
    return monthDays[clock->month];
  }
}

/**
 * Increment seconds
 * 
 * @param clock 
 * @return uint8_t day
 */
void clock_tick(_clock_t *clock) {
  clock->second++;
  if (clock->second > 59) {
    clock->second = 0;

    clock->minute++;
    if (clock->minute > 59) {
      clock->minute = 0;

      clock->hour++;
      if (clock->hour > 23) {
        clock->hour = 0;

        clock->day++;
        if (clock->day > calculateMonthDays(clock)) {
          clock->day = 0;

          clock->month++;
          if (clock->month > 12) {
            clock->month = 0;

            clock->year++;
            if (clock->year > 99) {
              clock->year = 0;
            }
          }
        }
      }
    }
  }
}

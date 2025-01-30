#include <debug.h>
#include "clock.h"

const uint8_t monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * Increment seconds
 * 
 * @param datetime 
 * @return uint8_t day
 */
void clock_increment(_clock_t *clock) {
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
        if (clock->day > monthDays[clock->month]) {
          clock->day = 0;

          clock->month++;
          if (clock->month > 12) {
            clock->month = 0;

            clock->year++;
          }
        }
      }
    }
  }
}


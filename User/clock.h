#ifndef _CLOCK_H
#define _CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define STATE_TIME  0x01
#define STATE_DATE  0x02

/* Defined enums */
enum _menu {
  time, // 
  date,
  alarm
};

enum _menu_time {
  set_hour,
  set_minute,
  set_second
};

enum _menu_date {
  set_day,
  set_month,
  set_year
};

/* Defined structs */
typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} _clock_t;

/* Defined functions */
void clock_increment(_clock_t *clock);

#ifdef __cplusplus
}
#endif

#endif
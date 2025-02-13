#ifndef _CLOCK_H
#define _CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif 

#define POSITION_DEFAULT 0
#define POSITION_HOUR    0
#define POSITION_MINUTE  1
#define POSITION_SECOND  2
#define POSITION_DAY     0
#define POSITION_MONTH   1
#define POSITION_YEAR    2

/* Defined enums */
enum _state {
  show_time, 
  show_date,
  setup_time,
  setup_date,
  setup_alarm_no,
  setup_alarm_time
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
uint8_t calculateMonthDays(_clock_t *clock);

#ifdef __cplusplus
}
#endif

#endif
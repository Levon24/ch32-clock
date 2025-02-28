#ifndef _CLOCK_H
#define _CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif 

///////////////////////////////////////////////////////////////////////////////
#define POSITION_DEFAULT 0
#define POSITION_HOUR    0
#define POSITION_MINUTE  1
#define POSITION_SECOND  2
#define POSITION_DAY     0
#define POSITION_MONTH   1
#define POSITION_YEAR    2

#define ALARMS           20

///////////////////////////////////////////////////////////////////////////////
enum _state {
  show_time, 
  show_date,
  setup_time,
  setup_date,
  setup_brightness,
  setup_alarm_no,
  setup_alarm_time,
  setup_alarm_music
};

///////////////////////////////////////////////////////////////////////////////
typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint8_t year;
} _clock_t;

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t melody;
} _alarm_t;

///////////////////////////////////////////////////////////////////////////////
void clock_tick();
uint8_t clock_monthDays();

///////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif
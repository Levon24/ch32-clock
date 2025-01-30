#ifndef _CLOCK_H
#define _CLOCK_H

#ifdef __cplusplus
 extern "C" {
#endif 

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
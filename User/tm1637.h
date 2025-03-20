#ifndef _TM1637_H
#define _TM1637_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Global defines */
#define LED7SEG_PORT  GPIOD
#define LED7SEG_CLK   GPIO_Pin_2
#define LED7SEG_DIO   GPIO_Pin_3

#define TM1637_DISPLAY_LENGTH   6
#define TM1637_BIT_DELAY_US     400
#define TM1637_BRIGHTNESS_MAX   7

#define TM1637_MINUS  0x40;
#define TM1637_DOT    0x80;

/* Exported functions */
void tm1637_writeSegments(const uint8_t *segments);
uint8_t tm1637_toDigit(uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
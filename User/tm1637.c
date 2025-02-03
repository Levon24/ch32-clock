#include <debug.h>
#include "tm1637.h"

#define TM1637_WRITE_DATA_TO_DISPLAY 0x40
#define TM1637_REGISTER_ADDRESS 0xC0
#define TM1637_DISPLAY_CONTROL 0x80

const uint8_t tm1637_maping[] = {2, 1, 0, 5, 4, 3};
const uint8_t tm1637_digits[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};

uint8_t tm1637_brightness = 0x08 | 0x07; // On + Brightness

/**
 * NOTE: on TM1637 reading keys should be slower than 250Khz (see datasheet p3)
 * for that reason the delay between reading bits should be more than 4us
 * When using a fast clock (such as ESP8266) a delay is needed to read bits correctly 
 */
void tm1637_delay() {
  Delay_Us(TM1637_BIT_DELAY_US);
}

/**
 * Start data sending
 */
void tm1637_start() {
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  tm1637_delay();
}

/**
 * Stop data sending
 */
void tm1637_stop() {
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
  tm1637_delay();
}

/**
 * Write data block
 * 
 * @param data 
 * @return acknowledge 
 */
u8 tm1637_write_byte(uint8_t data) {
  // Write 8 bit data
  for (uint8_t i = 0; i < 8; i++) {
    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
    tm1637_delay();
    
    if (data & 0x01) {
      GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
    } else {
      GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
    }
    tm1637_delay();

    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
    tm1637_delay();

    data = data >> 1;
  }

  // Wait for acknowledge
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_SET);
  tm1637_delay();

  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_SET);
  tm1637_delay();
  
  uint8_t acknowledge = GPIO_ReadInputDataBit(LED7SEG_PORT, LED7SEG_DIO);
  if (acknowledge == 0) {
    GPIO_WriteBit(LED7SEG_PORT, LED7SEG_DIO, Bit_RESET);
  }
  tm1637_delay();
  
  GPIO_WriteBit(LED7SEG_PORT, LED7SEG_CLK, Bit_RESET);
  tm1637_delay();
  
  return acknowledge;
}

/**
 * Write data raw 
 * 
 * @param segments 
 */
void tm1637_write_segments(const uint8_t *segments) {
  // Write command #1: Write data to display register
  tm1637_start();
  tm1637_write_byte(TM1637_WRITE_DATA_TO_DISPLAY);
  tm1637_stop();
  
  // Write command #2: The command is used to set the display register address.
  tm1637_start();
  tm1637_write_byte(TM1637_REGISTER_ADDRESS);
  
  // write the data bytes
  for (uint8_t pos = 0; pos < TM1637_DISPLAY_LENGTH; pos++) {
    uint8_t seg = tm1637_maping[pos];
    tm1637_write_byte(segments[seg]);
  }
  tm1637_stop();
  
  // Write command #3: Display control
  tm1637_start();
  tm1637_write_byte(TM1637_DISPLAY_CONTROL | tm1637_brightness);
  tm1637_stop();
}

/**
 * Set brightness level
 * 
 * @param level 
 */
void tm1637_set_brightness(uint8_t level) {

  tm1637_brightness = 0x08 | (level & 0x07);
}

/**
 * Convert byte to digit
 * 
 * @param value 
 * @return digit
 */
uint8_t tm1637_toDigit(uint8_t value) {
  return tm1637_digits[value];
}

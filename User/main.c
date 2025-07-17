#include "debug.h"
#include "tm1637.h"
#include "clock.h"
#include "rtttl.h"
#include "melody.h"

///////////////////////////////////////////////////////////////////////////////
#define _TIM2_CORECTION   0 // 4
#define _TIM2_PSC         ((SystemCoreClock / 1000) - 1)
#define _TIM2_ARR         ((1000 - 1) + _TIM2_CORECTION)

#define BUTTON_SETTINGS   GPIO_Pin_1
#define BUTTON_NEXT       GPIO_Pin_0
#define BUTTON_UP         GPIO_Pin_3
#define BUTTON_DOWN       GPIO_Pin_2
#define BUTTON_PRESSED    0

#define LED_PIN           GPIO_Pin_6

///////////////////////////////////////////////////////////////////////////////
extern _clock_t clock;

uint8_t segments[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t flash = 0;

enum _state state = display_time;
uint8_t position = 0;

extern _alarm_t alarms[ALARMS];
uint8_t alarm = 0;

extern uint8_t tm1637_brightness;

/**
 * Init GPIO Ports
 */
void GPIO_InitPorts(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
  
  // Buttons
  GPIO_InitTypeDef GPIO_Buttons = {0};
  GPIO_Buttons.GPIO_Pin = BUTTON_SETTINGS | BUTTON_NEXT | BUTTON_UP | BUTTON_DOWN;
  GPIO_Buttons.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Buttons.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &GPIO_Buttons);

  // TM1637
  GPIO_InitTypeDef GPIO_Display = {0};
  GPIO_Display.GPIO_Pin = LED7SEG_CLK | LED7SEG_DIO;
  GPIO_Display.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Display.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOD, &GPIO_Display);

  // Speaker, Led
  GPIO_InitTypeDef GPIO_Output = {0};
  GPIO_Output.GPIO_Pin = SPEAKER_PIN | LED_PIN;
  GPIO_Output.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Output.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOD, &GPIO_Output);
}

/**
 * Init Timers
 */
void TIM_InitTimers() {
  // Timer 1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitStruct1 = {0};
  NVIC_InitStruct1.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStruct1.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct1.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct1.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct1);

  // Timer 2
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
  TIM_TimeBaseInitTypeDef TIMBase_InitStruct2 = {0};
  TIMBase_InitStruct2.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMBase_InitStruct2.TIM_Prescaler = _TIM2_PSC;
  TIMBase_InitStruct2.TIM_Period = _TIM2_ARR;
  TIMBase_InitStruct2.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct2);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitStruct2 = {0};
  NVIC_InitStruct2.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct2.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct2.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct2);
  
  TIM_Cmd(TIM2, ENABLE);
}

/**
 * IRQ Timer 1 Update
 */
void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void) {
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
    rtttl_tick();

    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
  }
}

/**
 * IRQ Timer 2
 */
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    clock_tick();
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
  }
}

/**
 * Display time
 */
void displayTime(uint16_t buttons) {
  // display
  segments[0] = tm1637_toDigit(clock.hour / 10);
  segments[1] = tm1637_toDigit(clock.hour % 10) | TM1637_DOT;
  segments[2] = tm1637_toDigit(clock.minute / 10);
  segments[3] = tm1637_toDigit(clock.minute % 10) | TM1637_DOT;
  segments[4] = tm1637_toDigit(clock.second / 10);
  segments[5] = tm1637_toDigit(clock.second % 10);

  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = setup_time;
  } 
  
  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    state = display_date;
  } 
}

/**
 * Display date
 */
void displayDate(uint16_t buttons) {
  // display
  segments[0] = tm1637_toDigit(clock.day / 10);
  segments[1] = tm1637_toDigit(clock.day % 10) | TM1637_DOT;
  segments[2] = tm1637_toDigit(clock.month / 10);
  segments[3] = tm1637_toDigit(clock.month % 10) | TM1637_DOT;
  segments[4] = tm1637_toDigit(clock.year / 10);
  segments[5] = tm1637_toDigit(clock.year % 10);

  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = setup_date;
  } 
  
  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    state = display_time;
  }
}

/**
 * Setup time
 */
void setupTime(uint16_t buttons) {
  // display
  if (position == POSITION_HOUR && flash) {
    segments[0] = 0;
    segments[1] = 0;
  } else {
    segments[0] = tm1637_toDigit(clock.hour / 10);
    segments[1] = tm1637_toDigit(clock.hour % 10) | TM1637_DOT;
  }
  
  if (position == POSITION_MINUTE && flash) {
    segments[2] = 0;
    segments[3] = 0;
  } else {
    segments[2] = tm1637_toDigit(clock.minute / 10);
    segments[3] = tm1637_toDigit(clock.minute % 10) | TM1637_DOT;
  }
  
  if (position == POSITION_SECOND && flash) {
    segments[4] = 0;
    segments[5] = 0;
  } else {
    segments[4] = tm1637_toDigit(clock.second / 10);
    segments[5] = tm1637_toDigit(clock.second % 10);
  }
  
  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = setup_date;
    position = POSITION_DEFAULT;
  } 
  
  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    if (position == POSITION_SECOND) {
      position = POSITION_HOUR;
    } else {
      position++;
    }
  }
  
  // logic
  switch (position) {
    case POSITION_HOUR:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.hour == 23) {
          clock.hour = 0;
        } else {
          clock.hour++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.hour == 0) {
          clock.hour = 23;
        } else {
          clock.hour--;
        }
      }
      break;
    
    case POSITION_MINUTE:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.minute == 59) {
          clock.minute = 0;
        } else {
          clock.minute++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.minute == 0) {
          clock.minute = 59;
        } else {
          clock.minute--;
        }
      }
      break;

    case POSITION_SECOND:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.second == 59) {
          clock.second = 0;
        } else {
          clock.second++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.second == 0) {
          clock.second = 59;
        } else {
          clock.second--;
        }
      }
      break;
  }
}

/**
 * Setup date
 */
void setupDate(uint16_t buttons) {
  // display
  if (position == POSITION_DAY && flash) {
    segments[0] = 0;
    segments[1] = 0;
  } else {
    segments[0] = tm1637_toDigit(clock.day / 10);
    segments[1] = tm1637_toDigit(clock.day % 10) | TM1637_DOT;
  }

  if (position == POSITION_MONTH && flash) {
    segments[2] = 0;
    segments[3] = 0;
  } else {
    segments[2] = tm1637_toDigit(clock.month / 10);
    segments[3] = tm1637_toDigit(clock.month % 10) | TM1637_DOT;
  }
  
  if (position == POSITION_YEAR && flash) {
    segments[4] = 0;
    segments[5] = 0;
  } else {
    segments[4] = tm1637_toDigit(clock.year / 10);
    segments[5] = tm1637_toDigit(clock.year % 10);
  }
  
  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = setup_brightness;
    position = POSITION_DEFAULT;
  }

  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    if (position == POSITION_YEAR) {
      position = POSITION_DAY;
    } else {
      position++;
    }
  }
  
  // logic
  switch (position) {
    case POSITION_DAY:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.day == clock_monthDays()) {
          clock.day = 1;
        } else {
          clock.day++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.day == 1) {
          clock.day = clock_monthDays();
        } else {
          clock.day--;
        }
      }
      break;

    case POSITION_MONTH:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.month == 12) {
          clock.month = 1;
        } else {
          clock.month++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.month == 1) {
          clock.month = 12;
        } else {
          clock.month--;
        }
      }
      break;

    case POSITION_YEAR:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (clock.year == 99) {
          clock.year = 0;
        } else {
          clock.year++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (clock.year == 0) {
          clock.year = 99;
        } else {
          clock.year--;
        }
      }
      break;
  }
}

/**
 * Setup brightness
 */
void setupBrightness(uint16_t buttons) {
  // display
  segments[0] = 0b01111111; // B
  segments[1] = 0b01110111; // R
  segments[2] = 0;
  segments[3] = 0;
  segments[4] = 0;
  segments[5] = flash ? tm1637_toDigit(tm1637_brightness) : 0;

  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = setup_alarm_num;
  }
  
  if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
    if (tm1637_brightness < TM1637_BRIGHTNESS_MAX) {
      tm1637_brightness++;
    }
  }

  if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
    if (tm1637_brightness > 0) {
      tm1637_brightness--;
    }
  }
}

/**
 * Setup alarm num
 * 
 */
void setupAlarmNum(uint16_t buttons) {
  // display
  segments[0] = 0b01110111; // A
  segments[1] = 0b00111000; // L
  segments[2] = 0;
  segments[3] = 0;
  if (flash) {
    segments[4] = 0;
    segments[5] = 0;
  } else {
    segments[4] = tm1637_toDigit(alarm / 10);
    segments[5] = tm1637_toDigit(alarm % 10);
  }
  
  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = display_time;
  }

  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    state = setup_alarm_time;
  }
  
  if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
    if (alarm < (ALARMS - 1)) {
      alarm++;
    }
  }

  if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
    if (alarm > 0) {
      alarm--;
    }
  }
}

/**
 * Setup alarm time
 */
void setupAlarmTime(uint16_t buttons) {
  // display
  segments[0] = 0b00000111; // T
  segments[1] = 0b00000001; // 
  if (position == POSITION_HOUR && flash) {
    segments[2] = 0;
    segments[3] = 0;
  } else {
    uint8_t hour = alarms[alarm].hour;
    segments[2] = tm1637_toDigit(hour / 10);
    segments[3] = tm1637_toDigit(hour % 10) | TM1637_DOT;
  }
  if (position == POSITION_MINUTE && flash) {
    segments[4] = 0;
    segments[5] = 0;
  } else {
    uint8_t minute = alarms[alarm].minute;
    segments[4] = tm1637_toDigit(minute / 10);
    segments[5] = tm1637_toDigit(minute % 10) | TM1637_DOT;
  }
  
  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = display_time;
    position = POSITION_DEFAULT;
  } 
  
  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    if (position == POSITION_MINUTE) {
      state = setup_alarm_music;
      position = POSITION_DEFAULT;
    } else {
      position++;
    }
  }
  
  // logic
  switch (position) {
    case POSITION_HOUR:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (alarms[alarm].hour == 23) {
          alarms[alarm].hour = 0;
        } else {
          alarms[alarm].hour++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (alarms[alarm].hour == 0) {
          alarms[alarm].hour = 23;
        } else {
          alarms[alarm].hour--;
        }
      }
      break;
      
    case POSITION_MINUTE:
      if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
        if (alarms[alarm].minute == 59) {
          alarms[alarm].minute = 0;
        } else {
          alarms[alarm].minute++;
        }
      }
      if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
        if (alarms[alarm].minute == 0) {
          alarms[alarm].minute = 59;
        } else {
          alarms[alarm].minute--;
        }
      }
      break;
  }
}

/**
 * Setup alarm music
 */
void setupAlarmMusic(uint16_t buttons) {
  // display
  segments[0] = 0b00110111; // M
  segments[1] = 0b00110111; // M
  segments[2] = 0;
  segments[3] = 0;
  if (flash) {
    segments[4] = 0;
    segments[5] = 0;
  } else {
    uint8_t melody = alarms[alarm].melody;
    segments[4] = tm1637_toDigit(melody / 10);
    segments[5] = tm1637_toDigit(melody % 10);
  }
  
  // buttons
  if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
    state = display_time;
  }

  if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
    state = setup_alarm_num;
  }
  
  if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
    if (alarms[alarm].melody < (MELODIES - 1)) {
      alarms[alarm].melody++;
      rtttl_play(alarms[alarm].melody);
    }
  }
  
  if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
    if (alarms[alarm].melody > 0) {
      alarms[alarm].melody--;
      rtttl_play(alarms[alarm].melody);
    }
  }
}

/**
 * Main function
 */
int main(void) {
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  SystemCoreClockUpdate();
  Delay_Init();

#if (SDI_PRINT == SDI_PR_OPEN)
  SDI_Printf_Enable();
#else
  USART_Printf_Init(115200);
#endif
  printf("SystemClk: %d\r\n", SystemCoreClock);
  printf("ChipID: %08x\r\n", DBGMCU_GetCHIPID());
  printf("GPIO Clock TEST\r\n");
    
  GPIO_InitPorts();
  TIM_InitTimers();

  //tm1637_set_brightness(5);
  //tm1637_write_segments(segments);
  rtttl_play(0);

  while (1) {
    uint16_t buttons = GPIO_ReadInputData(GPIOC);
    switch (state) {
      case display_time:
        displayTime(buttons);
        break;
    
      case display_date:
        displayDate(buttons);
        break;

      case setup_time:
        setupTime(buttons);
        break;

      case setup_date:
        setupDate(buttons);
        break;

      case setup_brightness:
        setupBrightness(buttons);
        break;

      case setup_alarm_num:
        setupAlarmNum(buttons);
        break;

      case setup_alarm_time:
        setupAlarmTime(buttons);
        break;

      case setup_alarm_music:
        setupAlarmMusic(buttons);
        break;
    }
    tm1637_writeSegments(segments);
    
    // LED
    GPIO_WriteBit(GPIOD, GPIO_Pin_6, (flash == 1) ? Bit_SET : Bit_RESET);

    // Update
    flash = (flash == 0) ? 1 : 0;

    Delay_Ms(95);
  }
}

#include "debug.h"
#include "tm1637.h"
#include "clock.h"

/* Global defines */
#define _TIM2_ARR   (1000 - 1)
#define _TIM2_PSC   ((SystemCoreClock / 1000) - 1)

#define BUTTON_SETTINGS   GPIO_Pin_3
#define BUTTON_NEXT       GPIO_Pin_2
#define BUTTON_UP         GPIO_Pin_7
#define BUTTON_DOWN       GPIO_Pin_6
#define BUTTON_PRESSED    0

/* Global Variables */
_clock_t clock = {14, 12, 59, 30, 1, 25};
uint8_t segments[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t flash = 0;

enum _state state = show_time;
uint8_t position = 0;

/* IRQ Handlers */
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * Init GPIO Ports
 */
void GPIO_InitPorts(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  
  // TM1637
  GPIO_InitTypeDef GPIO_InitPortC01 = {0};
  GPIO_InitPortC01.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitPortC01.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitPortC01.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &GPIO_InitPortC01);

  // Buttons
  GPIO_InitTypeDef GPIO_InitPortC23 = {0};
  GPIO_InitPortC23.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitPortC23.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitPortC23.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &GPIO_InitPortC23);

  // LED
  GPIO_InitTypeDef GPIO_InitPortD0 = {0};
  GPIO_InitPortD0.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitPortD0.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitPortD0.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOD, &GPIO_InitPortD0);
}

/**
 * Init Timer #2
 */
void TIM_InitTimer2() {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
  TIM_TimeBaseInitTypeDef TIMBase_InitStruct = {0};
  TIMBase_InitStruct.TIM_Period = _TIM2_ARR;
  TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIMBase_InitStruct.TIM_Prescaler = _TIM2_PSC;
  TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitStruct = {0};
  NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
  
  TIM_Cmd(TIM2, ENABLE);
}

/**
 * IRQ Timer 2
 */
void TIM2_IRQHandler(void) {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    clock_increment(&clock);
    
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
  }
}

/**
 * Main function
 * 
 * @return int 
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
  TIM_InitTimer2();

  //tm1637_set_brightness(5);
  //tm1637_write_segments(segments);

  while (1) {
    Delay_Ms(100);

    uint16_t buttons = GPIO_ReadInputData(GPIOC);

    switch (state) {
      case show_time:
        segments[0] = tm1637_toDigit(clock.hour / 10);
        segments[1] = tm1637_toDigit(clock.hour % 10) | TM1637_DOT;
        segments[2] = tm1637_toDigit(clock.minute / 10);
        segments[3] = tm1637_toDigit(clock.minute % 10) | TM1637_DOT;
        segments[4] = tm1637_toDigit(clock.second / 10);
        segments[5] = tm1637_toDigit(clock.second % 10);
        
        if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
          state = setup_time;
        } 
        if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
          state = show_date;
        } 
        break;
    
      case show_date:
        segments[0] = tm1637_toDigit(clock.day / 10);
        segments[1] = tm1637_toDigit(clock.day % 10) | TM1637_DOT;
        segments[2] = tm1637_toDigit(clock.month / 10);
        segments[3] = tm1637_toDigit(clock.month % 10) | TM1637_DOT;
        segments[4] = tm1637_toDigit(clock.year / 10);
        segments[5] = tm1637_toDigit(clock.year % 10);

        if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
          state = setup_date;
        } 
        if ((buttons & BUTTON_NEXT) == BUTTON_PRESSED) {
          state = show_time;
        }
        break;

      case setup_time:
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
        break;

      case setup_date:
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
        switch (position) {
          case POSITION_DAY:
            if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
              if (clock.day == calculateMonthDays(&clock)) {
                clock.day = 1;
              } else {
                clock.day++;
              }
             }
             if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
              if (clock.day == 1) {
                clock.day = calculateMonthDays(&clock);
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
        break;

      case setup_brightness: {
          uint8_t b = tm1637_getBrightness();

          segments[0] = 0b01111111; // B
          segments[1] = 0b01110111; // R
          segments[2] = 0;
          segments[3] = 0;
          segments[4] = 0;
          if (position == POSITION_DEFAULT && flash) {
            segments[5] = 0;
          } else {
            segments[5] = tm1637_toDigit(b);
          }
          
          if ((buttons & BUTTON_SETTINGS) == BUTTON_PRESSED) {
            state = show_time;
            position = POSITION_DEFAULT;
          } 
          if ((buttons & BUTTON_UP) == BUTTON_PRESSED) {
            if (b < 7) {
              tm1637_setBrightness(++b);
            }
          }
          if ((buttons & BUTTON_DOWN) == BUTTON_PRESSED) {
            if (b > 1) {
              tm1637_setBrightness(--b);
            }
          }
        }
        break;
    }
    tm1637_writeSegments(segments);
    
    // LED
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, (flash == 1) ? Bit_SET : Bit_RESET);

    // Update 
    if (flash == 0) {
      flash = 1;
    } else {
      flash = 0;
    }
  }
}

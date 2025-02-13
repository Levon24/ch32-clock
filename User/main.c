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

/* Global Variables */
_clock_t clock = {14, 12, 59, 30, 1, 25};
uint8_t segments[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t flash = 0;
uint8_t light = 0;

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
    Delay_Ms(250);

    if ((flash & 0x01) && light) {
      segments[0] = 0;
    } else {
      segments[0] = tm1637_toDigit(clock.hour / 10);
    }
    if ((flash & 0x02) && light) {
      segments[1] = 0;
    } else {
      segments[1] = tm1637_toDigit(clock.hour % 10) | TM1637_DOT;
    }

    segments[2] = tm1637_toDigit(clock.minute / 10);
    segments[3] = tm1637_toDigit(clock.minute % 10) | TM1637_DOT;
    segments[4] = tm1637_toDigit(clock.second / 10);
    segments[5] = tm1637_toDigit(clock.second % 10);
    tm1637_write_segments(segments);

    uint16_t buttons = GPIO_ReadInputData(GPIOC);
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, (buttons & BUTTON_UP) ? Bit_SET : Bit_RESET);

    // Update 
    if (light == 0) {
      light = 1;
    } else {
      light = 0;
    }
  }
}

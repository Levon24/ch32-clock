#include "debug.h"
#include "tm1637.h"
#include "clock.h"

/* Global defines */
#define _TIM2_ARR (1000 - 1)
#define _TIM2_PSC ((SystemCoreClock / 1000) - 1)

/* Global Variables */
_clock_t clock = {14, 12, 59, 30, 1, 25};
uint8_t segments[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/* IRQ Handlers */
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/**
 * Init GPIO Port C
 */
void GPIO_InitPortC(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitPort = {0};
  GPIO_InitPort.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitPort.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitPort.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &GPIO_InitPort);
}

/**
 * Init GPIO Port D
 */
void GPIO_InitPortD(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  
  GPIO_InitTypeDef GPIO_InitPort = {0};
  GPIO_InitPort.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitPort.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitPort.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOD, &GPIO_InitPort);
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
    
  GPIO_InitPortC();
  GPIO_InitPortD();

  TIM_InitTimer2();

  //tm1637_set_brightness(5);
  //tm1637_write_segments(segments);

  uint8_t port = 0;

  while (1) {
    Delay_Ms(250);

    segments[0] = tm1637_toDigit(clock.hour / 10);
    segments[1] = tm1637_toDigit(clock.hour % 10) | tm1637_getDot();
    segments[2] = tm1637_toDigit(clock.minute / 10);
    segments[3] = tm1637_toDigit(clock.minute % 10) | tm1637_getDot();
    segments[4] = tm1637_toDigit(clock.second / 10);
    segments[5] = tm1637_toDigit(clock.second % 10);
    tm1637_write_segments(segments);

    printf("Time: %02d/%02d/%02d %02d:%02d:%02d\r\n", clock.day, clock.month, clock.year, clock.hour, clock.minute, clock.second);

    GPIO_WriteBit(GPIOD, GPIO_Pin_0, (port == 0) ? (port = Bit_SET) : (port = Bit_RESET));
  }
}

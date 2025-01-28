#include "debug.h"
#include "tm1637.h"

/* Global define */

/* Global Variable */
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

void GPIO_Ports_Init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitTypeDef gpio_InitPortC = {0};
  gpio_InitPortC.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  gpio_InitPortC.GPIO_Mode = GPIO_Mode_Out_PP;
  gpio_InitPortC.GPIO_Speed = GPIO_Speed_30MHz;
  GPIO_Init(GPIOC, &gpio_InitPortC);
}

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
  printf("GPIO TM1637 TEST\r\n");
    
  GPIO_Ports_Init();
  
  while (1) {
    Delay_Ms(1000);
    printf("Time: %d:%d:%d\r\n", hours, minutes, seconds);
  }
}

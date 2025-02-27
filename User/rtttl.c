#include "debug.h"
#include "rtttl.h"
#include "melody.h"

#define _FREQ_OUT   (20000 /* Hz */ * 2 /* Ticks */)
#define _FREQ_TICKS (_FREQ_OUT / 2)
#define _FREQ_LEN   (2 /* Ticks */ * 60 /* Per minute */ * 4)
#define _TIM1_PSC   ((SystemCoreClock / _FREQ_OUT) - 1)

#define DURATION      64
#define SILENT_FREQ   1000

/* Nota -> Freq */
const uint16_t freq[] = {
//  P   C     C#    D     D#    E     F     F#    G     G#    A     A#    B
    0,  131,  139,  147,  156,  165,  175,  185,  196,  208,  220,  233,  247,  0,  0,  0,
    0,  262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,  0,  0,  0,
    0,  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,  0,  0,  0,
    0,  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 0,  0,  0,
    0,  2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 0,  0,  0,
};

const _melody_t *melody;
uint8_t pos;

uint32_t ticks;
uint32_t length;
uint8_t speaker;

extern const _melody_t *melodies[];

/**
 * Setup tim #1 to generate frequeancy
 * @param f 
 */
void setup(uint16_t f) {
  TIM_Cmd(TIM1, DISABLE);

  TIM_TimeBaseInitTypeDef TIMBase_InitStruct1 = {0};
  TIMBase_InitStruct1.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMBase_InitStruct1.TIM_Prescaler = _TIM1_PSC;
  TIMBase_InitStruct1.TIM_Period = ((_FREQ_TICKS / f) - 1);
  TIMBase_InitStruct1.TIM_CounterMode = TIM_CounterMode_Up;
  TIMBase_InitStruct1.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIMBase_InitStruct1);

  TIM_Cmd(TIM1, ENABLE);
}

/**
 * Start play
 * @param nota 
 */
void calculate() {
  uint16_t f = freq[melody->notes[pos].nota];
  if (f > 0) {
    length = (((uint32_t) f) * _FREQ_LEN * melody->notes[pos].duration) / DURATION / melody->tempo;
    speaker = 1;
    //printf("Duration: %d, Nota: %d -> Freq: %d, Length: %d.\r\n", melody->notes[pos].duration, melody->notes[pos].nota, f, length);
    setup(f);
  } else {
    length = (((uint32_t) SILENT_FREQ) * _FREQ_LEN * melody->notes[pos].duration) / DURATION / melody->tempo;
    speaker = 0;
    setup(SILENT_FREQ);
  }
}

/**
 * Tick handler
 */
void rtttl_tick() {
  if (melody == NULL) {
    return;
  }

  if (melody->notes[pos].duration == 0) {
    melody = NULL;
    pos = 0;
    ticks = 0;

    GPIO_WriteBit(SPEAKER_PORT, SPEAKER_PIN, Bit_RESET);
    TIM_Cmd(TIM1, DISABLE);
    
    return;
  }

  if (ticks < length) {
    GPIO_WriteBit(GPIOD, GPIO_Pin_2, (speaker && (ticks & 0x01) > 0) ? Bit_SET : Bit_RESET);
    ticks++;
  } else {
    if (ticks > 0) {
      ticks = 0;
      pos++;
    }
    
    calculate(); 
  }
}

/**
 * Play melody 
 * @param num 
 */
void rtttl_play(uint8_t num) {
  melody = melodies[46];
  pos = 0;

  calculate();
}

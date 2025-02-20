#include "debug.h"
#include "rtttl.h"

#define SILENT_FREQ 1000

/* Nota -> Freq */
const uint16_t freq[] = {
//  P   C     C#    D     D#    E     F     F#    G     G#    A     A#    B
    0,  262,  277,  294,  311,  329,  349,  370,  392,  415,  440,  460,  494,  0,  0,  0,
    0,  523,  554,  587,  622,  659,  699,  740,  784,  830,  880,  920,  988,  0,  0,  0,
    0,  785,  831,  881,  933,  989,  1048, 1110, 1176, 1245, 1320, 1380, 1482, 0,  0,  0,
    0,  1046, 1108, 1175, 1244, 1318, 1397, 1480, 1568, 1660, 1760, 1840, 1976, 0,  0,  0
};

// Алиса - Трасса Е95
const _melody_t alisa_trassa_e95 = {
  4, 135, {
    4,  G|O2,  8,  E|O2,   16, P,     16, G|O2,   8,  F_|O2, 16, P,      16, D|O2,  4,  A|O1,
    8,  B|O1,  16, P,      16, C|O2,  8,  D|O2,   8,  G|O2,  8,  F_|O2,  16, P,     16, A|O2,
    16, G|O2,  16, F_|O2,  16, E|O2,  16, D|O2,   4,  G|O2,  8,  E|O2,   16, P,     16, G|O2,
    8,  F_|O2, 16, P,      16, D|O2,  4,  A|O1,   8,  B|O1,  16, P,      16, C|O2,  8,  D|O2,
    8,  G|O2,  8,  F_|O2,  16, P,     16, A|O2,   16, G|O2,  16, F_|O2,  16, E|O2,  16, D|O2,
    2,  E|O2,  0, 0
  }
};

const _melody_t *melody;
uint8_t pos;

uint32_t ticks;
uint32_t length;
uint8_t speaker;

/**
 * Setup tim #1 to generate frequeancy
 * @param f 
 */
void setup(uint16_t f) {
  TIM_Cmd(TIM1, DISABLE);

  TIM_TimeBaseInitTypeDef TIMBase_InitStruct1 = {0};
  TIMBase_InitStruct1.TIM_ClockDivision = TIM_CKD_DIV1;
  TIMBase_InitStruct1.TIM_Prescaler = _TIM1_PSC;
  TIMBase_InitStruct1.TIM_Period = f / 2; // freq * 2
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
    length = (((f * 2 * melody->duration) / melody->notes[pos].duration) * 60) / melody->tempo;
    speaker = 1;
    setup(f);
  } else {
    length = (((SILENT_FREQ * 2 * melody->duration) / melody->notes[pos].duration) * 60) / melody->tempo;
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
  melody = &alisa_trassa_e95;
  pos = 0;

  calculate();
}
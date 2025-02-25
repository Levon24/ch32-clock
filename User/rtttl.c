#include "debug.h"
#include "rtttl.h"

#define _FREQ_OUT   80000
#define _FREQ_TICKS (_FREQ_OUT / 2)
#define _FREQ_LEN   (2 * 60 * 4) // 2 ticks * 60 sec * 4
#define _TIM1_PSC   ((SystemCoreClock / _FREQ_OUT) - 1)

#define DURATION      64
#define SILENT_FREQ   1000

/* Nota -> Freq */
const uint16_t freq[] = {
//  P   C     C#    D     D#    E     F     F#    G     G#    A     A#    B
    0,  262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,  0,  0,  0,
    0,  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,  0,  0,  0,
    0,  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 0,  0,  0,
    0,  2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 0,  0,  0,
};

// Мобильник (из Бумера)
const _melody_t mobil_bumer = {
  180, {
    8,	E|O2,		16,	G|O2,		16,	P|O2,		8,	G|O2,		16,	E|O2,		16,	P|O2,		8,	A|O2,		8,	G|O2,
    8,	A|O2,		8,	G|O2,		8,	A|O2,		8,	G|O2,		8,	A|O2,		8,	G|O2,		8,	A|O2,		16,	B|O2
  }
};

// Белые розы
const _melody_t white_roses = {
  120, {
    8,	B|O1,		8,	B|O1,		8,	B|O1,		16,	B|O1,		16,	F_|O1,	8,	P|O2,		8,	B|O1,		8,	B|O1,
    8,	B|O1,		16,	B|O1,		16,	F_|O1,	8,	P|O2,		16,	B|O1,		16,	G|O1,		8,	F_|O1,	8,	G|O1,
    8,	F_|O1,	16,	G|O1,		8,	P|O2,		8,	P|O2,		8,	P|O2,		8,	A|O1,		8,	A|O1,		8,	A|O1,
    16,	A|O1,		16,	E|O1,		8,	P|O2,		8,	A|O1,		8,	A|O1,		8,	A|O1,		16,	A|O1,		16,	E|O1,
    8,	P|O2,		16,	A|O1,		16,	F|O1,		8,	E|O1,		8,	F|O1,		8,	E|O1,		16,	F|O1
  }
};

// ДДТ - Оcень
const _melody_t ddt_autumn = {
  120, {
    8,	E|O2,		4,	C|O2,		12,	B|O1,		8,	A|O1,		12,	E|O2,		12,	B|O1,		4,	B|O1,		8,	C|O2,
    12,	B|O1,		48,	A|O1,		8,	A|O1,		4,	A|O1,		12,	A|O1,		8,	A|O1,		12,	A|O1,		8,	A|O1,
    4,	C_|O2,	12,	E|O2,		12,	G|O2,		48,	F|O2,		12,	D|O2,		4,	D|O2,		8,	D|O2,		8,	D|O2,
    8,	G|O2,		8,	F|O2,		12,	E|O2,		12,	D|O2,		8,	E|O2,		8,	E|O2,		4,	E|O2,		8,	D|O2,
    12,	C|O2,		16,	A|O1,		8,	P|O2,		8,	B|O1,		8,	B|O1,		8,	G|O2,		8,	F|O2,		8,	E|O1,
    4,	E|O1,		8,	E|O2,		12,	C|O2,		12,	B|O1,		32,	A|O1
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
  TIMBase_InitStruct1.TIM_Period = (_FREQ_TICKS / f);
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
    printf("Duration: %d, Nota: %d -> Freq: %d, Length: %d.\r\n", melody->notes[pos].duration, melody->notes[pos].nota, f, length);
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
  melody = &ddt_autumn;
  pos = 0;

  calculate();
}

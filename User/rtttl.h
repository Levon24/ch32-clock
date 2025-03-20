#ifndef _RTTTL_H
#define _RTTTL_H

#ifdef __cplusplus
 extern "C" {
#endif

#define SPEAKER_PORT  GPIOD
#define SPEAKER_PIN   GPIO_Pin_0

/* Octava */
#define O0  0x00
#define O1  0x10
#define O2  0x20
#define O3  0x30
#define O4  0x40

/* Nota */
#define P   0x00 // Pause
#define C   0x01 // Do
#define C_  0x02 // Do #
#define D   0x03 // Re
#define D_  0x04 // Re #
#define E   0x05 // Mi
#define F   0x06 // Fa
#define F_  0x07 // Fa #
#define G   0x08 // So
#define G_  0x09 // So #
#define A   0x0A // La
#define A_  0x0B // La #
#define B   0x0C // Ti
#define H   0x0C // Si

typedef struct {
  uint8_t duration;
  uint8_t nota;
} _nota_t;

/* Functions */
void rtttl_tick();
void rtttl_play(uint8_t num);

#ifdef __cplusplus
}
#endif

#endif
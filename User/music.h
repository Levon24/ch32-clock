#ifndef _MUSIC_H
#define _MUSIC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Octava */
#define O0  0x00
#define O1  0x10
#define O2  0x20
#define O3  0x30
#define O4  0x40

/* Nota */
#define P   0x00 // Pause
#define C   0x01 // Do
#define D   0x02 // Re
#define E   0x03 // Mi
#define F   0x04 // Fa
#define G   0x05 // So
#define A   0x06 // La
#define B   0x07 // Ti

typedef struct {
  uint8_t nota;
  uint8_t duration;
} _nota_t;


#ifdef __cplusplus
}
#endif

#endif
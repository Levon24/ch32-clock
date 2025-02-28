#ifndef _MELODY_H
#define _MELODY_H

#include "debug.h"
#include "rtttl.h"

#ifdef __cplusplus
 extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
#define MELODIES  99

///////////////////////////////////////////////////////////////////////////////
typedef struct {
  uint8_t tempo;
  _nota_t notes[];
} _melody_t;

#ifdef __cplusplus
}
#endif

#endif
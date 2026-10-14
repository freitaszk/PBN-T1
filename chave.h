#ifndef CHAVE_H
#define CHAVE_H

#include <stdint.h>

#define SAL_CONFUSAO 0xC0FFEE01u
#define SAL_CANAIS   0x0BADF00Du
#define SAL_PIXELS   0x5EED1234u

typedef struct {
  uint32_t estado;
} Gerador;

void gerador_init(Gerador* g, const char* chave, uint32_t sal);
uint32_t proximo(Gerador* g);
uint32_t sorteia(Gerador* g, uint32_t limite);

#endif

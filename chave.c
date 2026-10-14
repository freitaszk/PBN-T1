#include "chave.h"

// nao troca isso por rand(), o rand do windows e do linux dao numeros diferentes
// e ai a imagem bagunçada num pc nao volta no outro

void gerador_init(Gerador* g, const char* chave, uint32_t sal) {
  uint32_t h = 2166136261u ^ sal;
  for (const unsigned char* p = (const unsigned char*)chave; *p; p++) {
    h ^= *p;
    h *= 16777619u;
  }
  // xorshift com estado 0 fica preso no 0 pra sempre
  g->estado = h ? h : 0x9E3779B9u;
}

uint32_t proximo(Gerador* g) {
  uint32_t x = g->estado;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  g->estado = x;
  return x;
}

uint32_t sorteia(Gerador* g, uint32_t limite) {
  return proximo(g) % limite;
}

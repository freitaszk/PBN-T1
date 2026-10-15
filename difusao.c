#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bagunca.h"
#include "chave.h"

static unsigned int* gera_permutacao(const char* chave, size_t n) {
  Gerador g;
  unsigned int* perm = malloc(n * sizeof(unsigned int));
  if (!perm) {
    printf("Sem memoria pra permutacao\n");
    exit(1);
  }

  gerador_init(&g, chave, SAL_PIXELS);
  for (size_t i = 0; i < n; i++) *(perm + i) = (unsigned int)i;

  for (size_t i = n - 1; i > 0; i--) {
    size_t j = sorteia(&g, (uint32_t)(i + 1));
    unsigned int tmp = *(perm + i);
    *(perm + i) = *(perm + j);
    *(perm + j) = tmp;
  }
  return perm;
}

static void embaralha_pixels(Img* img, const char* chave, int desfaz) {
  size_t n = total_pixels(img);
  unsigned int* perm = gera_permutacao(chave, n);
  Pixel* aux = malloc(total_bytes(img));
  if (!aux) {
    printf("Sem memoria pro buffer auxiliar\n");
    exit(1);
  }

  Pixel* origem = img->pixels;
  // nao da pra fazer no lugar sem estragar, por isso o aux
  for (size_t i = 0; i < n; i++) {
    if (!desfaz)
      *(aux + *(perm + i)) = *(origem + i);
    else
      *(aux + i) = *(origem + *(perm + i));
  }

  memcpy(img->pixels, aux, total_bytes(img));
  free(aux);
  free(perm);
}

void difunde(Img* img, const char* chave) {
  embaralha_pixels(img, chave, 0);
}

void desdifunde(Img* img, const char* chave) {
  embaralha_pixels(img, chave, 1);
}

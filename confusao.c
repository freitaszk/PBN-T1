#include "bagunca.h"
#include "chave.h"

static void monta_sbox(Gerador* g, unsigned char* sbox, unsigned char* inversa) {
  for (int i = 0; i < 256; i++) sbox[i] = (unsigned char)i;

  for (int i = 255; i > 0; i--) {
    int j = (int)sorteia(g, (uint32_t)i + 1);
    unsigned char tmp = sbox[i];
    sbox[i] = sbox[j];
    sbox[j] = tmp;
  }

  for (int i = 0; i < 256; i++) inversa[sbox[i]] = (unsigned char)i;
}

void confunde(Img* img, const char* chave) {
  Gerador g;
  unsigned char sbox[256], inversa[256];

  gerador_init(&g, chave, SAL_CONFUSAO);
  monta_sbox(&g, sbox, inversa);

  unsigned char* p = (unsigned char*)img->pixels;
  unsigned char* fim = p + total_bytes(img);
  unsigned char anterior = (unsigned char)proximo(&g);

  //c = sbox[v + k] ^ c_anterior
  for (; p < fim; p++) {
    unsigned char k = (unsigned char)proximo(&g);
    *p = sbox[(unsigned char)(*p + k)] ^ anterior;
    anterior = *p;
  }
}

void desconfunde(Img* img, const char* chave) {
  Gerador g;
  unsigned char sbox[256], inversa[256];

  //tem que gerar tudo na **mesma ordem do confunde, senao os k's nao batem
  gerador_init(&g, chave, SAL_CONFUSAO);
  monta_sbox(&g, sbox, inversa);

  unsigned char* p = (unsigned char*)img->pixels;
  unsigned char* fim = p + total_bytes(img);
  unsigned char anterior = (unsigned char)proximo(&g);

  //v = inversa[c ^ c_anterior] - k
  for (; p < fim; p++) {
    unsigned char k = (unsigned char)proximo(&g);
    unsigned char c = *p;
    *p = (unsigned char)(inversa[c ^ anterior] - k);
    anterior = c;
  }
}

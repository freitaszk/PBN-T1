#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include <stb_image.h>
#include <stb_image_write.h>
#pragma GCC diagnostic pop

#include "imagem.h"

size_t total_pixels(const Img* pic) {
  return (size_t)pic->width * pic->height;
}

size_t total_bytes(const Img* pic) {
  return total_pixels(pic) * sizeof(Pixel);
}

void load(const char* nome, Img* pic) {
  //esse 3 no final obriga a vir sempre RGB, se vier png com alpha o struct pixel quebra tudo
  pic->pixels = (Pixel*)stbi_load(nome, &pic->width, &pic->height, &pic->channels, 3);
  if (!pic->pixels) {
    printf("Erro de leitura: %s\n", stbi_failure_reason());
    exit(1);
  }
  pic->channels = 3;
  printf("Load: %s %d x %d (%lu bytes em %p)\n", nome, pic->width, pic->height,
         (unsigned long)total_bytes(pic), (void*)pic->pixels);
}

void salva(const char* nome, const Img* pic) {
  // tem que ser png, jpg perde informacao e ai nao volta nunca mais
  if (!stbi_write_png(nome, pic->width, pic->height, 3, pic->pixels, 0)) {
    printf("Erro ao gravar %s\n", nome);
    exit(1);
  }
  printf("Gravou: %s\n", nome);
}

Img copia(const Img* origem) {
  Img nova = *origem;
  nova.pixels = malloc(total_bytes(origem));
  if (!nova.pixels) {
    printf("Sem memoria pra copiar a imagem\n");
    exit(1);
  }
  memcpy(nova.pixels, origem->pixels, total_bytes(origem));
  return nova;
}

void libera(Img* pic) {
  free(pic->pixels);
  pic->pixels = NULL;
}

void mostra_bytes(const char* titulo, const Img* pic, int quantos) {
  unsigned char* p = (unsigned char*)pic->pixels;
  // imagem minuscula (tipo 1x1) tem menos bytes que isso, ai estoura
  if ((size_t)quantos > total_bytes(pic)) quantos = (int)total_bytes(pic);
  printf("%-12s [%p] ", titulo, (void*)p);
  for (int i = 0; i < quantos; i++) printf("%02X ", *(p + i));
  printf("\n");
}

size_t compara(const Img* a, const Img* b) {
  if (a->width != b->width || a->height != b->height) return (size_t)-1;

  unsigned char* pa = (unsigned char*)a->pixels;
  unsigned char* pb = (unsigned char*)b->pixels;
  unsigned char* fim = pa + total_bytes(a);
  size_t diferentes = 0;

  while (pa < fim) {
    if (*pa++ != *pb++) diferentes++;
  }
  return diferentes;
}

#ifndef IMAGEM_H
#define IMAGEM_H

#include <stddef.h>

typedef struct {
  unsigned char r, g, b;
} Pixel;

typedef struct {
  int width, height;
  int channels;
  Pixel* pixels;
} Img;

void load(const char* nome, Img* pic);
void salva(const char* nome, const Img* pic);
Img copia(const Img* origem);
void libera(Img* pic);

size_t total_pixels(const Img* pic);
size_t total_bytes(const Img* pic);

void mostra_bytes(const char* titulo, const Img* pic, int quantos);
size_t compara(const Img* a, const Img* b);

#endif

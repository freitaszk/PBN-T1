/*
Compilar: mingw32-make (windows) ou make (linux/mac), ou CTRL+SHIFT+B no VS Code

Uso:
  bagunceitor <imagem> <chave>                  -> demo completa (gera tudo e confere)
  bagunceitor -b <imagem> <chave> [saida.png]   -> so bagunça
  bagunceitor -r <baguncada.png> <chave> [saida.png] -> so recupera
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bagunca.h"
#include "imagem.h"

#define BYTES_MOSTRADOS 12

static void uso(void) {
  printf("bagunceitor <imagem> <chave>\n");
  printf("bagunceitor -b <imagem> <chave> [saida.png]\n");
  printf("bagunceitor -r <baguncada.png> <chave> [saida.png]\n");
}

static void bagunca(Img* img, const char* chave) {
  confunde(img, chave);
  difunde(img, chave);
}

static void recupera(Img* img, const char* chave) {
  //D^-1 primeiro, depois C^-1
  desdifunde(img, chave);
  desconfunde(img, chave);
}

static int demo(const char* arquivo, const char* chave) {
  Img original, so_conf, so_dif, imagem, lida;

  load(arquivo, &original);
  mostra_bytes("original", &original, BYTES_MOSTRADOS);

  //esses dois aqui usamos pra mostrar no video cada etapa separada
  so_conf = copia(&original);
  confunde(&so_conf, chave);
  salva("so_confusao.png", &so_conf);

  so_dif = copia(&original);
  difunde(&so_dif, chave);
  salva("so_difusao.png", &so_dif);

  imagem = copia(&original);
  confunde(&imagem, chave);
  mostra_bytes("confundida", &imagem, BYTES_MOSTRADOS);
  difunde(&imagem, chave);
  mostra_bytes("baguncada", &imagem, BYTES_MOSTRADOS);
  salva("baguncada.png", &imagem);

  recupera(&imagem, chave);
  mostra_bytes("recuperada", &imagem, BYTES_MOSTRADOS);
  salva("recuperada.png", &imagem);

  size_t dif_memoria = compara(&original, &imagem);

  // relendo do disco pra provar que o png bagunçado sozinho ja basta
  load("baguncada.png", &lida);
  recupera(&lida, chave);
  size_t dif_arquivo = compara(&original, &lida);

  printf("\nBytes comparados: %lu\n", (unsigned long)total_bytes(&original));
  printf("Diferencas (memoria): %lu\n", (unsigned long)dif_memoria);
  printf("Diferencas (lendo baguncada.png): %lu\n", (unsigned long)dif_arquivo);
  printf(dif_memoria == 0 && dif_arquivo == 0 ? "RECUPERACAO EXATA :)\n"
                                                : "DEU RUIM, nao recuperou igual\n");

  libera(&original);
  libera(&so_conf);
  libera(&so_dif);
  libera(&imagem);
  libera(&lida);

  return dif_memoria == 0 && dif_arquivo == 0 ? 0 : 2;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    uso();
    return 1;
  }

  if (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-r") == 0) {
    if (argc < 4) {
      uso();
      return 1;
    }
    int recuperar = argv[1][1] == 'r';
    const char* saida = argc > 4 ? argv[4] : (recuperar ? "recuperada.png" : "baguncada.png");
    Img img;

    load(argv[2], &img);
    if (recuperar)
      recupera(&img, argv[3]);
    else
      bagunca(&img, argv[3]);
    salva(saida, &img);
    libera(&img);
    return 0;
  }

  return demo(argv[1], argv[2]);
}

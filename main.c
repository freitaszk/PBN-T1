#include <stdio.h>
#include <stdlib.h>

#include "bagunca.h"
#include "imagem.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    printf("bagunceitor <imagem> <chave>\n");
    return 1;
  }

  Img original, imagem;

  load(argv[1], &original);
  imagem = copia(&original);

  difunde(&imagem, argv[2]);
  salva("so_difusao.png", &imagem);

  desdifunde(&imagem, argv[2]);
  salva("recuperada.png", &imagem);

  printf("Diferencas: %lu\n", (unsigned long)compara(&original, &imagem));

  libera(&original);
  libera(&imagem);
  return 0;
}

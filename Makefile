# Makefile multiplataforma

PROG = bagunceitor

# Compilador
CC = gcc

# Flags
CFLAGS = -Iinclude -Wall -g -O2
LDFLAGS  =
LIBS     =

# Detecta SO and e ajusta extensao se necessario
ifeq ($(OS),Windows_NT)
    # Windows (MinGW)
    EXEEXT  = .exe
    OSNAME  = Windows
    LIBS    += -lm
else
    UNAME_S := $(shell uname -s)
    EXEEXT  =
    ifeq ($(UNAME_S),Linux)
        OSNAME  = Linux
        LIBS    += -lm 
    endif
    ifeq ($(UNAME_S),Darwin)
        OSNAME  = macOS
        CFLAGS  += -Wno-deprecated
        LIBS    += -lm
    endif
endif

# Fontes
FONTES = main.c imagem.c chave.c confusao.c difusao.c
OBJ = $(FONTES:.c=.o)

# Target (com extensao se necessario)
TARGET = $(PROG)$(EXEEXT)

# Regras
all: $(TARGET)
	@echo "Build completado para $(OSNAME). Saida: $(TARGET)"

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# stb_image eh pesado, entao so recompila o imagem.o quando precisa
main.o: imagem.h bagunca.h
imagem.o: imagem.h
chave.o: chave.h
confusao.o: bagunca.h chave.h imagem.h
difusao.o: bagunca.h chave.h imagem.h

clean:
ifeq ($(OS),Windows_NT)
	del /Q $(subst /,\,$(OBJ)) $(TARGET) 2>nul || true
else
	rm -f $(OBJ) $(TARGET)
endif

# Mostra infos do SO detectado
info:
	@echo "SO detectado: $(OSNAME)"
	@echo "Compilador:   $(CC)"
	@echo "CFLAGS:       $(CFLAGS)"
	@echo "LIBS:        $(LIBS)"
	@echo "Target:       $(TARGET)"

.PHONY: all clean info

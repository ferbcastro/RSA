#ifndef __CODIFICA__
#define __CODIFICA__
#include <wchar.h>

#define NUM_CODIGOS 120

struct contSimbolo {
    wchar_t c;
    int qtd;
};

struct letra {
    wchar_t c;
    int cod;
    char tam;
};

struct nodo {
    char ehFolha;
    wchar_t c;
    char num;
    struct nodo* fesq;
    struct nodo* fdir;
};

struct fila {
    struct nodo *ini;
    int tam;
};

char codifica(wchar_t* string, int* cods);
int cod(wchar_t caracter);
wchar_t decod(int numero);
#endif
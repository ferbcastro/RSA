#include <wchar.h>

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
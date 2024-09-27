#include "codificacao.h"
#include <string.h>
#include <stdlib.h>

void push(struct fila* f, struct nodo* nodo) {
    nodo->fdir = NULL;
    if (f->tam == 0) {
        f->ini = nodo;
    } else {
        struct nodo* n = f->ini;
        if (n->fdir == NULL) {
            if (nodo->num < n->num) {
                nodo->fdir = n;
                f->ini = nodo;
            } else {
                n->fdir = nodo;
            }
        } else {
            while (n->fdir != NULL && nodo->num > n->fdir->num) {
                n = n->fdir;
            }
            nodo->fdir = n->fdir;
            n->fdir = nodo;
        }
    }
    ++f->tam;
}

void pop(struct fila* f, struct nodo** nodo) {
    struct nodo* n;
    for(n = f->ini; n->fdir != NULL; n = n->fdir);
    --f->tam;
    *nodo = n;
    return;
}

void preencheDict(struct letra dict[], struct nodo* cabeca, int* i, char *t) {
    if (cabeca->ehFolha) {
        int j = 0;
        while (dict[j].c != cabeca->c) {
            j++;
        }
        dict[j].cod = *i;
        dict[j].tam = *t;
        return;
    }
    
    ++(*t);
    (*i) = (*i) << 1;
    preencheDict(dict, cabeca->fesq, i, t);
    (*i) |= 1;
    preencheDict(dict, cabeca->fdir, i, t);
    (*i) = (*i) >> 1;
    --(*t);
}

void codifica(struct letra dict[], char* str) {
    int tamStr = strlen(str);
    wchar_t* strWide = malloc(tamStr * sizeof(wchar_t));
    struct contSimbolo vetCont[100];
    int qtdSimbolos = 0;
    int pos;

    mbstowcs(strWide, str, tamStr);
    for (int i = 0; i < 100; i++) {
        vetCont[i].qtd = 0;
    }
    
    tamStr = wcslen(strWide);
    for (int i = 0; i < tamStr; i++) {
        for (pos = 0; pos < qtdSimbolos; pos++) {
            if (strWide[i] == vetCont[pos].c) {
                break;
            }
        }

        vetCont[pos].c = strWide[i];
        ++vetCont[pos].qtd;
        if (pos == qtdSimbolos) {
            qtdSimbolos++;
        }
    }

    struct nodo* ptrNodo;
    struct fila fila;
    fila.tam = 0;
    for (int i = 0; i < qtdSimbolos; i++) {
        ptrNodo = malloc(sizeof(struct nodo));
        ptrNodo->ehFolha = 1;
        ptrNodo->c = vetCont[i].c;
        ptrNodo->num = vetCont[i].qtd;
        push(&fila, ptrNodo);
    }

    struct nodo *ele1, *ele2;
    while (1) {
        pop(&fila, &ele1);
        if (fila.tam <= 1) break;
        pop(&fila, &ele2);
        ptrNodo = malloc(sizeof(struct nodo));
        ptrNodo->ehFolha = 0;
        ptrNodo->num = ele1->num + ele2->num;
        ptrNodo->fesq = ele1;
        ptrNodo->fdir = ele2;
        push(&fila, ptrNodo);
    }

    int k = 0;
    char tam = 1;
    preencheDict(dict, ele1, &k, &tam);
}

void decodifica(struct letra dict[], wchar_t c, int *cod, int *tam) {
    for (int i = 0; i < 100; i++) {
        if (dict[i].c == c) {
            *cod = dict[i].cod;
            *tam = dict[i].tam;
            return;
        }
    }
    return;
}
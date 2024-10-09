#include "codificacao.h"
#include <stddef.h>
#include <string.h>
#include <wchar.h>

#include <math.h>

// Associa um wchar_t com a posicao do vetor de codigos
int cod(wchar_t caracter) {
    if (caracter == L' ') {
        return NUM_CODIGOS - 1;
    } else if (caracter == L'!') {
        return NUM_CODIGOS - 2;
    } if (caracter >= 0x41 && caracter <= 0x5a) {
        return caracter - L'A';
    } else if (caracter <= 0x7a) {
        return caracter - L'a' + 26;
    } else {
        return caracter - L'À' + 52;
    }
}

// Associa posicao no vetor de codigos com um wchar_t
wchar_t decod(int numero) {
    if (numero == NUM_CODIGOS - 1) {
        return L' ';
    } else if (numero == NUM_CODIGOS - 2) {
        return L'!';
    } else if (numero >= 0 && numero <= 25) {
        return numero + L'A';
    } else if (numero <= 51) {
        return numero + L'a' - 26;
    } else {
        return numero + L'À' - 52;
    }
}

// Gera um codigo para as letras com frequencia >= 1
char codifica(wchar_t* string, int* cods) {
    int freqs[NUM_CODIGOS];
    memset(freqs, 0, NUM_CODIGOS * sizeof(int));
    size_t tam = wcslen(string);

    while (tam--) {
        ++freqs[cod(string[tam])];
    }

    int cont = 0;
    for (int i = 0; i < NUM_CODIGOS; i++) {
        if (freqs[i] > 0) {
            cont++;
            cods[i] = cont;
            #ifdef DEBUG
            wprintf(L"i = %d cods[i] = %d", i, cods[i]);
            #endif
        }
    }

    if (cont == 0) {
        return 0;
    }

    return (int)(log2(cont)) + 1;
}
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <wchar.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include "codificacao.h"

int codigos[NUM_CODIGOS];

typedef unsigned long long ull;

struct PrivateKey {
    ull e;
    ull n;
};

int translateHex(char c) {
    if (c >= 0x61 && c <= 0x66) {
        return c - 'a' + 10;
    } else {
        return c - '0';
    }  
}

int main(int argc, char** argv) {
    struct PrivateKey priv;

    setlocale(LC_ALL, "");

    int opt;
    while ((opt = getopt(argc, argv, "e:n:")) != -1) {
        switch (opt) {
            case 'n':
                priv.n = strtol(optarg, NULL, 10);
                break;
            case 'e':
                priv.e = strtol(optarg, NULL, 10);
                break;
            default:
                return 1;
        }
    }

    char* str = NULL, *ptr;
    wchar_t auxstr[1024];
    size_t sizestr;
    unsigned long bits = 0, it, cod, num, mult;
    mpz_t base, exp, mod;

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_set_ui(exp, priv.e);
    mpz_set_ui(mod, priv.n);

    // Obtem numero de bits da codificacao
    // Mult gera um numero com 1s de tamanho de bits
    FILE* output = fopen("output.txt", "r");
    sizestr = getline(&str, &sizestr, output);
    for(ptr = str; *ptr != ':'; ptr++) {
        bits = bits << 4;
        bits += translateHex(*ptr);
    }
    mpz_set_ui(base, bits);
    mpz_powm(base, base, exp, mod);
    bits = mpz_get_ui(base);
    mult = (1 << bits) - 1;

    // Decifra e guarda codigos em um vetor
    ptr += 2;
    while (*ptr != '\n') {
        it = cod = 0;
        for(; *ptr != ':'; ptr++) {
            it = it << 4;
            it += translateHex(*ptr);
        }
        ptr++;

        assert(*ptr != '\n');
        for(; *ptr != ':'; ptr++) {
            cod = cod << 4;
            cod += translateHex(*ptr);
        }
        mpz_set_ui(base, cod);
        mpz_powm(base, base, exp, mod);
        cod = mpz_get_ui(base);
        ptr++;

        codigos[it] = cod;
    }

    #ifdef DEBUG
    printf("bits = %ld\n", bits);
    for (int i = 0; i < NUM_CODIGOS; i++) {
        printf("codigo[%d] = %d\n", i, codigos[i]);
    }
    #endif

    int fimbloco, aux, i;
    free(str);
    str = NULL;
    sizestr = getline(&str, &sizestr, output);
    ptr = str;
    wprintf(L" ");
    while (*ptr != '\n') {
        // Bloco de letras eh descriptografado
        num = 0;
        for(; *ptr != ':'; ptr++) {
            num = num << 4;
            num += translateHex(*ptr);
        }
        mpz_set_ui(base, num);
        mpz_powm(base, base, exp, mod);
        num = mpz_get_ui(base);

        // O bloco eh separado nas letras
        // A convencao eh terminar com o codigo 0x0 ou maximo 8 bytes
        // O bloco entao eh impresso ao contrario
        fimbloco = it = 0;
        while (!fimbloco) {
            aux = 0;
            aux = num & mult;
            num = num >> bits;
            if (aux == 0) {
                while (it--) {
                    wprintf(L"%lc", auxstr[it]);
                }
                fimbloco = 1;
            } else {
                for (i = 0; i < NUM_CODIGOS; i++) {
                    if (codigos[i] == aux) {
                        break;
                    }
                }
                auxstr[it] = decod(i);
                it++;
            }
        }    

        ptr++;
    }
    wprintf(L"\n");
    
    free(str);
    fclose(output);
    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
}


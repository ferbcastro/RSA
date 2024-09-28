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
                break;
        }
    }

    char* str = NULL, *ptr;
    wchar_t auxstr[sizeof(unsigned long)];
    size_t sizestr;
    unsigned long bits = 0, it, cod, num;
    mpz_t base, exp, mod;

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_set_ui(exp, priv.e);
    mpz_set_ui(mod, priv.n);

    FILE* output = fopen("output.txt", "r");
    sizestr = getline(&str, &sizestr, output);
    for(ptr = str; *ptr != ':'; ptr++) {
        bits += translateHex(*ptr);
        bits = bits << 4;
    }
    bits = bits >> 4;
    mpz_set_ui(base, bits);
    mpz_powm(base, base, exp, mod);
    bits = mpz_get_ui(base);
    bits = (1 << bits) - 1;

    ptr += 2;
    while (*ptr != '\n') {
        it = cod = 0;
        for(; *ptr != ':'; ptr++) {
            it += translateHex(*ptr);
            it = it << 4;
        }
        it = it >> 4;
        ptr++;

        assert(*ptr != '\n');
        for(; *ptr != ':'; ptr++) {
            cod += translateHex(*ptr);
            cod = cod << 4;
        }
        cod = cod >> 4;
        mpz_set_ui(base, cod);
        mpz_powm(base, base, exp, mod);
        cod = mpz_get_ui(base);
        ptr++;

        codigos[it] = cod;
    }

    #define DEBUG
    #ifdef DEBUG
    printf("bits = %ld\n", bits);
    for (int i = 0; i < NUM_CODIGOS; i++) {
        printf("codigo[%d] = %d\n", i, codigos[i]);
    }
    #endif
    char fimbloco;
    sizestr = getline(&str, &sizestr, output);
    ptr = str;
    it = 0;
    while (*ptr != '\n') {
        num = 0;
        for(; *ptr != ':'; ptr++) {
            num += translateHex(*ptr);
            num = num << 4;
        }
        num = num >> 4;
        mpz_set_ui(base, num);
        mpz_powm(base, base, exp, mod);
        num = mpz_get_ui(base);

        fimbloco = 0;
        while (!fimbloco) {
            // codigo
        }    

        ptr++;
    }
    free(str);

    fclose(output);
    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
}


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <wchar.h>
#include <getopt.h>
#include <assert.h>
#include <locale.h>
#include "codificacao.h"

int codigos[NUM_CODIGOS];

typedef unsigned long long ull;

typedef struct PublicKey {
    ull d;
    ull n;
} PublicKey;

int main(int argc, char** argv) {
    char* str = NULL;
    wchar_t* widestr;
    size_t sizeStr = 0;
    PublicKey pub;

    srand(time(NULL));
    setlocale(LC_ALL, "");

    int opt;
    while ((opt = getopt(argc, argv, "n:d:")) != -1) {
        switch (opt) {
            case 'n':
                pub.n = strtol(optarg, NULL, 10);
                break;
            case 'd':
                pub.d = strtol(optarg, NULL, 10);
                break;
            case '?':
            default:
                return 1;
        }
    }

    mpz_t base, exp, mod;
    char bits;
    char *auxstr;

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_set_ui(exp, pub.d);
    mpz_set_ui(mod, pub.n);
    
    wprintf(L"\n Insira sua mensagem: ");
    sizeStr = getline(&str, &sizeStr, stdin);
    widestr = malloc(sizeof(wchar_t) * (sizeStr + 1));
    mbstowcs(widestr, str, sizeStr);
    sizeStr = wcslen(widestr) - 1;
    bits = codifica(widestr, codigos);
    assert(bits > 0);

    FILE* output = fopen("output.txt", "w");
    mpz_set_ui(base, (unsigned long)bits);
    mpz_powm(base, base, exp, mod);
    auxstr = mpz_get_str(NULL, 16, base);
    fprintf(output, "%s::", auxstr);
    free(auxstr);

    for(int i = 0; i < NUM_CODIGOS; i++) {
        if (codigos[i] != 0) {
            mpz_set_ui(base, (unsigned long)codigos[i]);
            mpz_powm(base, base, exp, mod);
            auxstr = mpz_get_str(NULL, 16, base);
            fprintf(output, "%x:%s:", i, auxstr);
            free(auxstr);
        }
    }
    fprintf(output, "\n");

    unsigned long tmp;
    char bytes;
    unsigned int it = 0;

    #ifdef DEBUG
    wprintf(L"%d\n", codigos[cod(widestr[it])]);
    for (int i = 0; i < sizeStr; i++) {
        wprintf(L"%d ", cod(widestr[i]));
    }
    #endif

    while (it < sizeStr) {
        bytes = sizeof(unsigned long) - 1;
        tmp = 0;
        tmp |= codigos[cod(widestr[it])];
        it++;
        while (it < sizeStr && bytes > 0) {
            tmp = tmp << bits;
            tmp |= codigos[cod(widestr[it++])];
            bytes--;
        }

        if (tmp >= pub.n) {
            tmp = tmp >> bits;
            it--;
        }

        mpz_set_ui(base, tmp);
        mpz_powm(base, base, exp, mod);
        auxstr = mpz_get_str(NULL, 16, base);
        fprintf(output, "%s:", auxstr);
        free(auxstr);
    }
    fprintf(output, "\n");

    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
    free(str);
    free(widestr);
    fclose(output);
}
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <wchar.h>
#include <getopt.h>
#include <assert.h>
#include <locale.h>

#define MAX_BLOCKS 1024
#define SIZE_UNSIGNED_LONG 8

typedef unsigned long long ull;

typedef struct PublicKey {
    ull d;
    ull n;
} PublicKey;

int main(int argc, char** argv) {
    char* str = NULL;
    size_t sizeStr = 0;
    int opt;
    PublicKey pub;

    srand(time(NULL));
    setlocale(LC_ALL, "");

    while ((opt = getopt(argc, argv, "n:d:")) != -1) {
        switch (opt) {
            case 'n':
                pub.n = strtol(optarg, NULL, 10);
                break;
            case 'd':
                pub.d = strtol(optarg, NULL, 10);
                break;
            default:
                break;
        }
    }

    printf("\n Insira sua mensagem: ");
    sizeStr = getline(&str, &sizeStr, stdin);
    FILE* output = fopen("output.txt", "w");
    unsigned long tmp;
    char *auxStr, bytes, aleat = 1;
    unsigned int it = 0, ini;
    mpz_t base, exp, mod;

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_set_ui(exp, pub.d);
    mpz_set_ui(mod, pub.n);
    while (it < sizeStr) {
        tmp = 0;
        tmp |= str[it];
        it++;
        while (tmp < pub.n && it < sizeStr && aleat) {
            tmp = tmp << 8;
            tmp |= str[it];
            it++;
        }

        if (tmp >= pub.n) {
            tmp = tmp >> 8;
            it--;
        }

        mpz_set_ui(base, tmp);
        mpz_powm(base, base, exp, mod);
        auxStr = mpz_get_str(NULL, 16, base);
        fprintf(output, "%s:", auxStr);
        free(auxStr);
    }
    printf("\n");

    free(str);
}
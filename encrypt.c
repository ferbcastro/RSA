#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <wchar.h>
#include <getopt.h>
#include <assert.h>

#define MAX_BLOCKS 1024

typedef unsigned long long ull;

typedef struct PublicKey {
    ull d;
    ull n;
} PublicKey;

ull powMod (ull a, ull b, ull m) {
    mpz_t base, exp, mod;

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_set_ui(base, a);
    mpz_set_ui(exp, b);
    mpz_set_ui(mod, m);
    mpz_powm(mod, base, exp, mod);

    return mpz_get_si(mod);
}

int main(int argc, char** argv) {
    char* str;
    size_t sizeStr;
    int opt;
    PublicKey pub;

    while ((opt = getopt(argc, argv, "n:d:"))) {
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

    ull blocks[MAX_BLOCKS];
    size_t aux, blockSize = sizeof(ull);
    puts("Insira a mensagem: ");
    getline(&str, &sizeStr, stdin);
    
    int strIt = 0, blocksIt = 0;
    while (strIt < sizeStr) {
        blocks[blocksIt] = 0;
        aux = blockSize;
        while (aux > 0 && blocks[blocksIt] < pub.n) {
            blocks[blocksIt] |= str[strIt];
            blocks[blocksIt] = blocks[blocksIt] << 8;
            strIt++;
            aux -= 8;
        }
        blocksIt++;
    }

    FILE* output = fopen("output.txt", "r");
    assert(output != NULL);
    unsigned int tmp;
    for (int i = 0; i < blocksIt; i++) {
        blocks[i] = powMod(blocks[i], pub.d, pub.n);
        tmp = (unsigned int)(blocks[i] & 0xFFFFFFFF);
        fprintf(output, "%x", tmp);
        tmp = (unsigned int)(blocks[i] >> 32);
        fprintf(output, "%x", tmp);
    }

    free(str);
    fclose(output);
}
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <wchar.h>
#include <unistd.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>

#define TAM_MAX 1025

typedef unsigned long long ull;

struct PrivateKey {
    ull e;
    ull n;
};

int extended_gcd(int a, int b) {
    int old_cofa, old_cofb, new_cofa, new_cofb;
    int aux1, aux2, div;

    old_cofa = 1;
    old_cofb = 0;
    new_cofa = 0;
    new_cofb = 1;
    while (b != 0) {
        div = a / b;
        aux1 = old_cofa - div * new_cofa;
        aux2 = old_cofb - div * new_cofb;
        old_cofa = new_cofa;
        old_cofb = new_cofb;
        new_cofa = aux1;
        new_cofb = aux2;

        aux1 = a % b;
        a = b;
        b = aux1; 
    }

    return old_cofb;
}

int main(int argc, char** argv) {
    struct PrivateKey privKey;
    ull d, phi, primeP, primeQ;
    char opt;

    while ((opt = getopt(argc, argv, "p:q:d:e:")) != -1) {
        switch (opt) {
            case 'p':
                primeP = strtol(optarg, NULL, 10);
                break;
            case 'q':
                primeQ = strtol(optarg, NULL, 10);
                break;
            case 'd':
                d = strtol(optarg, NULL, 10);
                break;
            case 'e':
                privKey.e = strtol(optarg, NULL, 10);
                break;
            default:
                break;
        }
    }

    privKey.n = primeP * primeQ;
    phi = (primeP - 1) * (primeQ - 1);

    FILE* output = fopen("output.txt", "w");
    assert(output != NULL);
    char* str, aux[3];
    size_t strSize;
    getline(&str, &strSize, output);
    wchar_t* wideStr = malloc(TAM_MAX * sizeof(wchar_t));
    assert(wideStr != NULL);

    int j;
    for (int i = 0; i < strSize;) {
        j = i;
        
    }
}


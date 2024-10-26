#include "codificacao.h"

typedef unsigned long long ull;

typedef struct PublicKey {
    ull d;
    ull n;
} PublicKey;

char* encrypt (const char* str, const char* input_n, const char* input_d);
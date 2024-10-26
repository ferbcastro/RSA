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
#include "decrypt.h"

int translateHex(char c) {
    if (c >= 0x61 && c <= 0x66) {
        return c - 'a' + 10;
    } else {
        return c - '0';
    }  
}

char* decrypt(const char* input_n, const char* input_e) {
    struct PrivateKey priv;
    int codigos[NUM_CODIGOS];
    memset(codigos, 0, NUM_CODIGOS * sizeof(int));

    /*
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
    */

    char* str = NULL, *ptr, output_str_aux[64];
    char* output_str = malloc(1024);
    wchar_t auxstr[1024];
    size_t sizestr;
    unsigned long bits = 0, it, cod, num, mult;
    mpz_t base, exp, mod;

    output_str[0] = '\0';
    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    priv.e = strtoull(input_e, NULL, 10);
    priv.n = strtoull(input_n, NULL, 10);
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
    free(str);
    str = NULL;

    #ifdef DEBUG
    printf("bits = %ld\n", bits);
    for (int i = 0; i < NUM_CODIGOS; i++) {
        printf("codigo[%d] = %d\n", i, codigos[i]);
    }
    #endif

    int fimbloco, aux, i;
    sizestr = getline(&str, &sizestr, output);
    ptr = str;
    // wprintf(L" ");
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
        wchar_t temp;
        fimbloco = it = 0;
        while (!fimbloco) {
            aux = 0;
            aux = num & mult;
            num = num >> bits;
            if (aux == 0) {
                if (it != 0) {
                    i = --it;
                    while (i > it / 2) {
                        temp = auxstr[i];
                        auxstr[i] = auxstr[it - i];
                        auxstr[it - i] = temp;
                        i--;
                    }
                    auxstr[it + 1] = '\0';
                    wcstombs(output_str_aux, auxstr, 64);
                    strcat(output_str, output_str_aux);
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
    //wprintf(L"\n");
    
    free(str);
    fclose(output);
    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);

    return output_str;
}
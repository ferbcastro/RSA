#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <getopt.h>
#include <assert.h>
#include <locale.h>
#include "encrypt.h"
#include "codificacao.h"


char* encrypt (const char* str, const char* input_n, const char* input_d) {
    wchar_t* widestr;
    size_t sizeStr = 0;
    PublicKey pub;
    int codigos[NUM_CODIGOS];
    memset(codigos, 0, NUM_CODIGOS * sizeof(int));
    
    /*
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
    */
    mpz_t base, exp, mod;
    char bits;
    char *auxstr, *output_str;
    output_str = malloc(1024);
    output_str[0] = '\0';

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);

    pub.n = strtoul(input_n, NULL, 10);
    pub.d = strtoul(input_d, NULL, 10);
    mpz_set_ui(exp, pub.d);
    mpz_set_ui(mod, pub.n);
    /*
    wprintf(L"\n Insira sua mensagem: ");
    str[sizeStr - 1] = '\0';
    wprintf(L"\n");
    */

    // Transforma string para formato wide, obtem codificacao e o numero minimo de
    // bits necessario
    sizeStr = strlen(str) + 1;
    widestr = malloc(sizeof(wchar_t) * (sizeStr));
    mbstowcs(widestr, str, sizeStr);
    sizeStr = wcslen(widestr);
    bits = codifica(widestr, codigos);
    assert(bits > 0);

    // Numero de bits eh criptofado e impresso em 'output.txt' em formato hexadecimal
    FILE* output = fopen("output.txt", "w");
    mpz_set_ui(base, (unsigned long)bits);
    mpz_powm(base, base, exp, mod);
    auxstr = mpz_get_str(NULL, 16, base);
    fprintf(output, "%s::", auxstr);
    free(auxstr);

    // Codificacao criada eh criptografada e impressa no arquivo (posicao:codigo:)
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

    // Insere o maximo possivel de simbolos em um ull
    // Criptografa o valor no ull
    // Imprime o bloco formado em hexadecimal no arquivo (bloco:)
    while (it < sizeStr) {
        bytes = sizeof(unsigned long) - 1;
        tmp = 0;
        tmp |= codigos[cod(widestr[it++])];
        while (tmp < pub.n && it < sizeStr && bytes > 0) {
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
        strcat(output_str, auxstr);
        fprintf(output, "%s:", auxstr);
        free(auxstr);
    }
    fprintf(output, "\n");

    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
    free(widestr);
    fclose(output);

    return output_str;
}
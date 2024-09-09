/* Autor: Fernando de Barros Castro - UFPR
 * Data: 
 * Descricao: Implementacao do algoritmo de chave pública RSA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define TAM_MAX 1024

int limpa_buffer() {
    while (fgetc(stdin) != '\n');
}

int main() {
    int p, q, d, e, phi;
    char *msg = malloc(TAM_MAX);
    long int n;
    size_t tam_msg;
    mpz_t pot;

    mpz_init(pot);
    scanf("%d%d%d", &p, &q, &d);
    limpa_buffer();
    n = p * q;

    fgets(msg, TAM_MAX - 1, stdin);
    tam_msg = strlen(msg);
    
    int i;
    for (i = 0; i < tam_msg; i++) {
        printf("vet[%d] = %d\n", i, msg[i]);
        mpz_set_ui(pot, msg[i]);
        mpz_pow_ui(pot, pot, d);
        mpz_mod_ui(pot, pot, n);
        msg[i] = mpz_get_si(pot);
    }

    for (int i = 0; i < tam_msg; i++) {
        mpz_set_ui(pot, msg[i]);
        mpz_pow_ui(pot, pot, 13);
        mpz_mod_ui(pot, pot, n);
        msg[i] = mpz_get_si(pot);
        printf("vet[%d] = %d\n", i, msg[i]);
    }
    
    mpz_clear(pot);

    return 0;
}
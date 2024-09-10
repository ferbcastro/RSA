/* Autor: Fernando de Barros Castro - UFPR
 * Data: 
 * Descricao: Implementacao do algoritmo de chave pública RSA
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <wchar.h>

#define TAM_MAX 1024

int extended_gcd(int a, int b) {
    int old_cofa, old_cofb, new_cofa, new_cofb;
    int aux1, aux2, div;

    old_cofa = 1;
    old_cofb = 0;
    new_cofa = 0;
    new_cofb = 1;
    while (b != 0) {
        div = a / b;
        aux1 = old_cofa + div * new_cofa;
        aux2 = old_cofb + div * new_cofb;
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

void input(int *ptr, wchar_t* nome) {
    wprintf(L"%ls = ", nome);
    scanf("%d", ptr);
    while (getchar() != '\n');
}

char* inputMsg(size_t *tam) {
    char *str;
    wprintf(L"Mensagem: ");
    getline(&str, tam, stdin);

    return str;
}

int main() {
    int p, q, d, e, phi;
    wchar_t* msg = malloc(TAM_MAX * sizeof(wchar_t));
    size_t tam_msg;
    mpz_t pot;
    char* tmp = NULL;
    long int n;
    int i;

    setlocale(LC_ALL, "");

    mpz_init(pot);

    input(&p, L"p");
    input(&q, L"q");
    input(&d, L"d");
    tmp = inputMsg(&tam_msg);
    e = extended_gcd((p - 1) * (q - 1), d);
    n = p * q;

    mbstowcs(msg, tmp, tam_msg);
    tam_msg = wcslen(msg) - 1;
    
    wprintf(L"\n");
    for (i = 0; i < tam_msg; i++) {
        mpz_set_ui(pot, msg[i]);
        mpz_pow_ui(pot, pot, d);
        mpz_mod_ui(pot, pot, n);
        msg[i] = mpz_get_si(pot);
        wprintf(L"%lc", msg[i]);
    }
    wprintf(L"\n");

    for (int i = 0; i < tam_msg; i++) {
        mpz_set_ui(pot, msg[i]);
        mpz_pow_ui(pot, pot, e);
        mpz_mod_ui(pot, pot, n);
        msg[i] = mpz_get_si(pot);
        wprintf(L"%lc", msg[i]);
    }
    wprintf(L"\n");

    mpz_clear(pot);
    free(tmp);
    free(msg);

    return 0;
}
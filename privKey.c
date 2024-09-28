#include <stdio.h>

long int extended_gcd(long int a, long int b) {
    long int old_cofa, old_cofb, new_cofa, new_cofb;
    long int aux1, aux2, div;

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

int main() {
    long int p, q, d, e, phi;

    printf(" p = ");
    scanf("%ld", &p);
    printf(" q = ");
    scanf("%ld", &q);
    printf(" d = ");
    scanf("%ld", &d);
    printf("\n");

    phi = (p - 1) * (q - 1);
    e = extended_gcd(phi, d);
    while (e < 0) {
        e += phi;
    }
    printf(" e = %ld\n", e);
    printf(" n = %ld\n", p * q);
}
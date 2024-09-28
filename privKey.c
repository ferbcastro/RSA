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
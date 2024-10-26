typedef unsigned long long ull;

struct PrivateKey {
    ull e;
    ull n;
};

char* decrypt(const char* input_n, const char* input_e);
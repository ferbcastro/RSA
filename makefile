CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lgmp -lm

OBJS1 = decrypt.o codificacao.o
OBJS2 = encrypt.o codificacao.o

all: msg rsa

msg:
	@echo "compiling..."

rsa: $(OBJS2) $(OBJS1)
	$(CC) -o encrypt $(OBJS2) $(CFLAGS) $(LDFLAGS)
	$(CC) -o decrypt $(OBJS1) $(CFLAGS) $(LDFLAGS)
	$(CC) -o privkey privKey.c

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
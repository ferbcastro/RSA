CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lgmp -lm
GTKFLAGS = `pkg-config gtk+-3.0 --libs --cflags`

OBJS = decrypt.o encrypt.o codificacao.o

all: msg rsa

msg:
	@echo "compiling..."

rsa: $(OBJS)
	$(CC) -o privkey privKey.c
	$(CC) -o ui ui.c $(OBJS) $(CFLAGS) $(LDFLAGS) $(GTKFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lgmp

OBJS1 = decrypt.o
OBJS2 = encrypt.o

all: msg rsa

msg:
	@echo "compiling..."

rsa: $(OBJS)
	$(CC) -o encrypt $(OBJS2) $(CFLAGS) $(LDFLAGS)
	$(CC) -o decrypt $(OBJS1) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
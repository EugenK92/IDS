CC = gcc
NAME = IDS
CFLAGS = -Wall -Werror -pedantic
LDFLAGS = -lssl -lcrypto
FILES = main.c checksum.c

IDS: $(FILES)
	$(CC) $(FILES) -o $(NAME) $(CFLAGS)

clean:
	rm $(NAME)

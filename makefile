CC = gcc
NAME = IDS
CFLAGS = -Wall -Werror -pedantic
LDFLAGS = -lssl -lcrypto
FILES = main.c checkfile.c

IDS: $(FILES)
	$(CC) $(FILES) -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(NAME)

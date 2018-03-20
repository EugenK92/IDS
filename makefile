CC = gcc
NAME = IDS
CFLAGS = -Wall -Werror -pedantic
LDFLAGS = -lssl -lcrypto -l sqlite3
FILES = main.c checkfile.c database.c

IDS: $(FILES)
	$(CC) $(FILES) -o $(NAME) $(CFLAGS) $(LDFLAGS)

debug: $(FILES)
	$(CC) $(FILES) -g -o $(NAME) $(CFLAGS) $(LDFLAGS)

clean:
	rm $(NAME)

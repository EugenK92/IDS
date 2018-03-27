CC = gcc
NAME = IDS
CFLAGS = -Wall -Werror -pedantic
LDFLAGS = -lssl -lcrypto -l sqlite3 -lxml2 -lsodium
LIBS = -I/usr/include/libxml2 -I/usr/local/lib/
FILES = main.c checkfile.c database.c xml.c hash.c

IDS: $(FILES)
	$(CC) $(FILES) -o $(NAME) $(CFLAGS) $(LIBS) $(LDFLAGS)

debug: $(FILES)
	$(CC) $(FILES) -g -o $(NAME) $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm $(NAME)

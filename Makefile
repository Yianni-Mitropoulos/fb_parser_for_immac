CC = gcc
CFLAGS = -Wall -Wextra

parser.exe: main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f parser.exe
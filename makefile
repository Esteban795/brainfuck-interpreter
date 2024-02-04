SRCDIR = src
HEADDIR = include
LIBDIR = ./src

CFLAGS = -W -Wall -Wextra -Wvla -fsanitize=address   -g -lm

DEPENDENCIES = 

build:
	gcc $(SRCDIR)/interpreter.c -o ./bin/brainfuck-interpreter $(DEPENDENCIES) $(CFLAGS)

run:
	./bin/brainfuck-interpreter

clean:
	rm ./bin/brainfuck-interpreter

all:
	make build
	make run
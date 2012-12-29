CC := gcc
INCLUDES := -I./include
ARGS := -Wall
SOURCES := src/cpu.c src/main.c

all:
	$(CC) $(ARGS) $(SOURCES) $(INCLUDES) -o main -g

# vim: tabstop=4:softtabstop=4:shiftwidth=4:noexpandtab

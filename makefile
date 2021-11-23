all: main

main: main.c
	gcc -omain main.c hpctimer.c -Wall -lm
all: main

main: main.c
	gcc -omain -Wall -mavx main.c hpctimer.c -march=native
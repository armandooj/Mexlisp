mexlisp: parsing.c libraries/mpc.c brain.c
	gcc -std=c99 -Wall parsing.c libraries/mpc.c brain.c -ledit -lm -o parsing
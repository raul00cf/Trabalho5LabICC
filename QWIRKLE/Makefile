colors.o: colors.c
	gcc -c colors.c -o colors.o 

buffer.o: buffer.c 
	gcc -c buffer.c -o buffer.o 

output.o: output.c
	gcc -c output.c -o output.o 

jogo.o: jogo.c
	gcc -c jogo.c -o jogo.o 

tabuleiro.o: tabuleiro.c
	gcc -c tabuleiro.c -o tabuleiro.o 

main.o: main.c 
	gcc -c main.c -o main.o 

qwirkle: main.o colors.o output.o jogo.o buffer.o tabuleiro.o
	gcc main.o colors.o output.o jogo.o buffer.o tabuleiro.o -o qwirkle

all: qwirkle

clean: 
	rm *.o

run: all
	./qwirkle 


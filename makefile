all:
	gcc -Ofast -DNDEBUG -Wno-unused-result main.c init.c bitboards.c hashkeys.c board.c move.c movegen.c makemove.c evaluation.c search.c sorcery.c tt.c -o main

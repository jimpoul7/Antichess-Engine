CFLAGS = -Ofast -DNDEBUG -Wno-unused-result -std=c99

all:
	gcc $(CFLAGS) main.c init.c bitboards.c hashkeys.c board.c move.c movegen.c makemove.c evaluation.c search.c sorcery.c tt.c -o main

gprof:
	gcc $(CFLAGS) -pg main.c init.c bitboards.c hashkeys.c board.c move.c movegen.c makemove.c evaluation.c search.c sorcery.c tt.c -o Profiling/gprof

clean:
	rm gprof gmon.out

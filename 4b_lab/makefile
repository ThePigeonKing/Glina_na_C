
run: main.c
	gcc -g main.c funcs.c filework.c -o main.out
	./main.out

dbg: main.c
	clang -fsanitize=address -g main.c funcs.c filework.c -o main.out
	lldb ./main.out

adtest: main.c
	clang -fsanitize=address -g main.c funcs.c filework.c -o main.out
	./main.out

.PHONY: reserve
reserve: *.c
	cp -f main.c structs.h funcs.c funcs.h utils.h makefile filework.c filework.h ./reserved

clean: *.out
	rm *.out
	rm *.tst


run: main.c
	gcc -g main.c funcs.c -o main.out
	./main.out

dbg: main.c
	clang -fsanitize=address -g main.c funcs.c -o main.out
	lldb ./main.out

adtest: main.c
	clang -fsanitize=address -g main.c funcs.c -o main.out
	./main.out
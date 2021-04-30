

run: main.c
	gcc -g main.c func.c work_with_files.c -o main.out
	./main.out

dbg: main.c
	clang -g -fsanitize=address main.c func.c work_with_files.c -o main.out
	lldb ./main.out

adtest: main.c
	clang -g -fsanitize=address main.c func.c work_with_files.c -o main.out
	./main.out

clean: *.out
	rm *.out
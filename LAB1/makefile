build: ragged_arr.out

ragged_arr.out: ragged_arr.c
	gcc -Wextra -Wall ragged_arr.c -g -o ragged_arr.out

.PHONY: test
test: ragged_arr.out
	cat t1 | ./ragged_arr.out

.PHONY: clean
clean: *.out
	rm *.out

.PHONY: memtest
memtest: ragged_arr.out
	clang -g -fsanitize=memory ragged_arr.c -o ragged_msan.out
	cat t1 | ./ragged_msan.out

.PHONY: addresstest
addresstest: ragged_arr.out
	clang -g -fsanitize=address ragged_arr.c -o ragged_asan.out
	cat t1 | ./ragged_asan.out

build:
	gcc -Wall -g main.c -o tema3
clean:
	rm -f tema3
run: build
	./tema3 input output
val:
	valgrind --leak-check=full --show-leak-kinds=all ./tema3 input output

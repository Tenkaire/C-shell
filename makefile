test: my_shell
	gcc -o test my_shell.o
my_shell.o: my_shell.s
	gcc -c my_shell.s
my_shell.s: my_shell.c
	gcc -S my_shell.c
clean:
	rm -f test *.o my_shell.s

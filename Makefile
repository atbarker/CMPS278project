all:
	gcc -o rollback utils.c
clean:
	rm *.o rollback

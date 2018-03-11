OBJ := rollback.o utils.o main.o
EXE := rollback
HEADERS := $(wildcard *.h)

all:
	gcc -g -o rollback rollback.c utils.c main.c -ldb

#$(OBJ):$(HEADERS)

clean:
	rm *.o rollback

spotless:
	rm *.o rollback
	rm *.db __db.* log.* 

.PHONY: clean

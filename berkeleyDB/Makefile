OBJ := rollback.o utils.o main.o
EXE := rollback
HEADERS := $(wildcard *.h)

all:
	gcc -g -o rollback rollback.c utils.c simpledb.c main.c -ldb -pthread

#$(OBJ):$(HEADERS)

clean:
	rm rollback

spotless:
	rm rollback
	rm *.db __db.* log.* 

.PHONY: clean

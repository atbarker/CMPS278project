OBJ := rollback.o utils.o main.o
EXE := rollback
HEADERS := $(wildcard *.h)

$(EXE):$(OBJ)
	gcc -g -o rollback rollback.c utils.c main.c -ldb

$(OBJ):$(HEADERS)

clean:
	rm *.o rollback

.PHONY: clean

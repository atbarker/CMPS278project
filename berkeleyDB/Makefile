OBJ := rollback.o utils.o main.o
EXE := rollback
HEADERS := $(wildcard *.h)

$(EXE):$(OBJ)
	gcc -o $@ $(OBJ) -ldb

$(OBJ):$(HEADERS)

clean:
	rm *.o rollback

.PHONY: clean

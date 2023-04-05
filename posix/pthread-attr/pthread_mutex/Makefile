
TARGE := pthread
SRC:= pthread.c

$(TARGE):$(SRC)
	gcc -o $@ $^ -lpthread -g
	gcc -o test -lpthread test.c

clean:
	rm -rf $(TARGE) test

PHONY: clean

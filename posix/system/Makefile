
TARGE := system
SRC:= system.c

$(TARGE):$(SRC)
	gcc -o $@ $^
	gcc -o test test.c

clean:
	rm -rf $(TARGE) test

PHONY: clean

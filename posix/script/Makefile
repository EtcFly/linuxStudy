
TARGE := script
SRC:= script.c

$(TARGE):$(SRC)
	gcc -o $@ $^
	gcc -o show show.c --std=c99

clean:
	rm -rf $(TARGE) show

PHONY: clean

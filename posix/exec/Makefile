
TARGE := exec
SRC:= exec.c

$(TARGE):$(SRC)
	gcc -o $@ $^
	gcc -o show show.c --std=c99

clean:
	rm -rf $(TARGE)

PHONY: clean

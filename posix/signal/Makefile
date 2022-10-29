
TARGE := signal
SRC:= signal.c

$(TARGE):$(SRC)
	gcc -o $@ $^
	gcc -o child_signal child_signal.c

clean:
	rm -rf $(TARGE) child_signal

PHONY: clean

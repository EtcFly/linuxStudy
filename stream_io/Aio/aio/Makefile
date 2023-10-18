
TARGE := aio
SRC := $(TARGE).c
SRC_O := $(SRC:%.c=%.o)

CLINK +=  -lrt

$(TARGE):$(SRC_O)
	gcc -o $@ $^ $(CLINK)

%.o:%.c
	gcc -o $@ -c $< 

clean:
	-rm -rf $(TARGE)

.PHONY: clean

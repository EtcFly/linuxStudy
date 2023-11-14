
TARGE := ftok
SRC := $(TARGE).c
SRC_O := $(SRC:%.c=%.o)

CLINK = 

$(TARGE):$(SRC_O)
	gcc -o $@ $^ $(CLINK)

%.o:%.c
	gcc -o $@ -c $<  -g

clean:
	-rm -rf $(TARGE) $(SRC_O)

.PHONY: clean

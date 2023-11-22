TARGE := sempost
TARGE_SUB := semrcv

SRC := $(TARGE).c
SUB_SRC := $(TARGE_SUB).c

SRC_O := $(SRC:%.c=%.o)
SUB_SRC_O := $(SUB_SRC:%.c=%.o)

CLINK = -lpthread

ALL : $(TARGE) $(TARGE_SUB)

$(TARGE):$(SRC_O)
	gcc -o $@ $^ $(CLINK)

$(TARGE_SUB):$(SUB_SRC_O)
	gcc -o $@ $^ $(CLINK)

%.o:%.c
	gcc -o $@ -c $<  -g

clean:
	-rm -rf $(TARGE) $(SRC_O) $(TARGE_SUB) $(SUB_SRC_O)

.PHONY: clean ALL

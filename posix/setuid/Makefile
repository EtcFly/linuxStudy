
TARGE := setuid
SRC:= setuid.c

$(TARGE):$(SRC)
	gcc -o $@ $^
	echo passwd | sudo -s chown  root:etcfly $@
	# echo passwd | sudo -S  chown  root:etcfly $@
	echo passwd | sudo -S  chmod u+s $@
	# gcc -o show show.c --std=c99

clean:
	rm -rf $(TARGE)

PHONY: clean

PROG = project03
OBJS = project03.o

%.o: %.c
	gcc -c -g -o $@ $<

$(PROG): $(OBJS)
	gcc -g -o $@ $<

clean:
	rm -rf $(PROG) $(OBJS)

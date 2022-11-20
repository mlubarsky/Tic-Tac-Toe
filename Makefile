PROG = tictactoe
OBJS = tictactoe.o

%.o: %.c
	gcc -c -g -o $@ $<

$(PROG): $(OBJS)
	gcc -g -o $@ $<

clean:
	rm -rf $(PROG) $(OBJS)
	

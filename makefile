CC = g++
EXEC = cylinder
OBJS = motzkin.o
LINK_FLAG = # empty
COMP_FLAG = -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(LINK_FLAG) $(OBJS) -o $@

motzkin.o: motzkin.cpp motzkin.h
	$(CC) -c $(COMP_FLAG) $*.cpp

clean: 
	rm -f $(OBJS) $(EXEC)
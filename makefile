CC = g++
EXEC = cylinder
OBJS = main.o motzkin.o
LINK_FLAG = -lgmpxx -lgmp
COMP_FLAG = -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(LINK_FLAG) $(OBJS) -o $@

motzkin.o: motzkin.cpp motzkin.h
	$(CC) -c $(COMP_FLAG) $*.cpp

main.o: main.cpp motzkin.h
	$(CC) -c $(COMP_FLAG) $*.cpp

clean: 
	rm -f $(OBJS) $(EXEC)
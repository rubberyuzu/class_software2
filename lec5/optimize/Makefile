CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lm
OBJS = main.o func.o optimize.o

optimize: optimize.o
	$(CC) optimize.o $(LDLIBS)

a.out:	$(OBJS)
	$(CC) $(OBJS) $(LDLIBS)

clean:
	rm -f *~ *.o a.out
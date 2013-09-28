CC = g++
CFLAGS = -O2
all:	bf astar sa

bf:		Bruteforce.cc
	$(CC) $(CFLAGS) Bruteforce.cc -o bf

astar:	Astar.cc
	$(CC) $(CFLAGS) -std=c++11 Astar.cc -o astar

sa:		SA.cc
	$(CC) $(CFLAGS) SA.cc -o sa

clean:
	rm -f sa a.out astar bf

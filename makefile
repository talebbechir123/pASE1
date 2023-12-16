CC=gcc 
CFLAGS=-g -Wall

OFLAGS=-O3

FILES=  main.c server.c server.h

OBJDIR=obj

# create a directory 


all: server client test server_thread server2 clean_obj 


test.o: src/test.c include/hashtable.h include/replication.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/test.c


test: test.o hashtable.o replication.o
	$(CC) $(CFLAGS) $(OFLAGS) -o test test.o hashtable.o replication.o

replication.o: src/replication.c include/replication.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/replication.c
server_thread.o: src/server_thread.c include/server.h include/replication.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/server_thread.c

server_thread: server_thread.o hashtable.o resp.o replication.o
	$(CC) $(CFLAGS) $(OFLAGS) -o server_thread server_thread.o hashtable.o resp.o replication.o

server2.o: src/server2.c include/server.h include/hashtable.h include/replication.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/server2.c

server2: server2.o hashtable.o resp.o replication.o
	$(CC) $(CFLAGS) $(OFLAGS) -o server2 server2.o hashtable.o resp.o replication.o
hashtable.o: src/hashtable.c include/hashtable.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/hashtable.c

server.o: server.c include/server.h include/hashtable.h include/replication.h
	$(CC) $(CFLAGS) $(OFLAGS) -c server.c

server: server.o hashtable.o resp.o replication.o
	$(CC) $(CFLAGS) $(OFLAGS) -o server server.o hashtable.o resp.o replication.o

resp.o: src/resp.c include/resp.h
	$(CC) $(CFLAGS) $(OFLAGS) -c src/resp.c
# client.h is in the include path
client.o: src/client.c include/server.h 
	$(CC) $(CFLAGS) $(OFLAGS) -c src/client.c



clean_obj:
	rm -f *.o 




clean:
	rm -f main *.o *~ *# *.gch *.swp *.out server client test server_thread server2


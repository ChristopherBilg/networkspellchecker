# -lpthread
# -pthread

output: main.o fifo_queue.o openlisten_fd.o
	gcc -o output main.o fifo_queue.o openlisten_fd.o -lpthread

main.o: main.c
	gcc -o main.o main.c -c -lpthread

fifo_queue.o: fifo_queue.c
	gcc -o fifo_queue.o fifo_queue.c -c

openlisten_fd.o: openlisten_fd.c
	gcc -o openlist_fd.o openlisten_fd.c -c

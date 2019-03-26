# -lpthread
# -pthread

output: main.o fifo_queue.o open_listenfd.o
	gcc -o output main.o fifo_queue.o open_listenfd.o -lpthread

main.o: main.c
	gcc -o main.o main.c -c -lpthread

fifo_queue.o: fifo_queue.c
	gcc -o fifo_queue.o fifo_queue.c -c

open_listenfd.o: open_listenfd.c
	gcc -o open_listenfd.o open_listenfd.c -c

main: semaphore.o cq.o main.o
	gcc -o main cq.o semaphore.o main.o -lpthread

main.o : main.c
	gcc -c -g semaphore.h main.c

semaphore.o : semaphore.h cq.h semaphore.c
	gcc -c -g semaphore.h semaphore.c

cq.o : cq.h cq.c
	gcc -c -g cq.h cq.c

clean :
	rm -rf *.o
	rm -rf *.h.gch
	rm -rf main


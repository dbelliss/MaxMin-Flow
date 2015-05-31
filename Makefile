router.out : QueueAr.o router.o RouterRunner.o vector.o  
	g++ -pg -ansi -Wall -g -o router.out QueueAr.o router.o RouterRunner.o vector.o  

QueueAr.o : QueueAr.cpp QueueAr.h 
	g++ -ansi -Wall -g -c QueueAr.cpp

router.o : router.cpp router.h RouterRunner.h 
	g++ -ansi -Wall -g -c router.cpp

RouterRunner.o : RouterRunner.cpp CPUTimer.h RouterRunner.h router.h 
	g++ -ansi -Wall -g -c RouterRunner.cpp

vector.o : vector.cpp vector.h 
	g++ -ansi -Wall -g -c vector.cpp

clean : 
	rm -f router.out QueueAr.o  router.o  RouterRunner.o  vector.o   

Experimenting Threads for Different Client Server Models

Implemented a Producer/Consumer model using a Queue

Producer will push events to the Queue

Consumer will pop the envents and execute it.


1.To Compile
   g++ -pthread simpleQ.c
2.To Debug
  g++ -pthread -g simpleQ.c

  gdb ./a.out


Observations :

 1. condition wait be enclosed in the equivalent of a "while loop" that checks the predicate.
	Ref: http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_cond_wait.html



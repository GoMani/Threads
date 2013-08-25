Experimenting Threads for Different Client Server Models

-->Implemented a Producer/Consumer model using a Queue

-->Producer will push events to the Queue

--> Consumer will pop the envents and execute it.


1.To Compile

   g++ -pthread simpleQ.c

2.To Debug
  g++ -pthread -g simpleQ.c
  gdb ./a.out


Observations :

 1. condition wait be enclosed in the equivalent of a "while loop" that checks the predicate.
	Ref: http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_cond_wait.html

	Because pthread_cond_signal() is allowed to wake up more than one thread waiting on the condition variable. Therefore you must double-check the condition once you wake up, because some other thread might have woken up and changed it ahead of you.If you know you have just one thread waiting, and you are sure nobody in the future will ever modify code elsewhere in the program to add another thread waiting, then you can use if. But you never know that for sure, so always use while.



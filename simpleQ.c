#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
struct Node
{
	struct Node * next;
	struct Node * prev;
	void* (*workTodo)(void* arg);
	int nodeId; 
};
long long int q_full_wait=0;
long long int q_empty_wait=0;
const int producerTh=1000;
const int consumerTh=100;
int counter=0;
int nodeCounter=0;
struct  Queue
{
	struct Node* head;
	struct Node* tail;
	int size;
	int capacity;
	pthread_cond_t q_full;
	pthread_mutex_t mutex;
	pthread_cond_t q_empty;


};

typedef struct Queue queue_t;
typedef void* (*functptr)(void* );

int flag=0;
void enqueue(queue_t *queue,functptr myfunc)
{
	
	/**************************************************
        *  Locks the Mutex Common for Producer/Consumer.. 
        *  only one can get into the critical region 
        ***************************************************/ 
        pthread_mutex_lock(&(queue->mutex));

	
	/**************************************************
 	*     If the Queue is full  (size = capacity)
 	*     a conditional wait is issued. Makes the current thread to sleep.
 	*     yields other thread to lock the mutex
 	*     Waken up by any signal to the conditional variable
 	*
 	*     Drawback: Need CPU cycles to bring back from sleep state to active  
	***************************************************/
	while(queue->size==queue->capacity)
	{
		q_full_wait++;
		pthread_cond_wait(&(queue->q_full),&(queue->mutex));
	}
	{
                Node *temp =(Node *) malloc(sizeof(struct Node));
		temp->nodeId=nodeCounter++; 
		temp->workTodo=myfunc;
		temp->next=NULL;
		++(queue->size);
		temp->prev=queue->tail;
		if(queue->tail)
			queue->tail->next=temp;
		queue->tail=temp;
		if(queue->head==NULL)
		{
			queue->head=queue->tail;
		}
	}
        /*******************************************************
 	* current thread releases the mutex
 	* Next thread waiting for the lock gets the mutex
 	* *****************************************************/
	pthread_mutex_unlock(&(queue->mutex)); 

	/***************************************************************************
 	* Sending a broadcost to all the threads waiting for the conditional variable
 	*
 	* In this case, wake up call to the consumer, as an Item is ready to pick
 	* **************************************************************************/

	pthread_cond_broadcast(&(queue->q_empty));
}
void dequeue(queue_t *queue)
{

	/**************************************************
        *  Locks the Mutex Common for Producer/Consumer.. 
        *  only one can get into the critical region 
        ***************************************************/ 
	pthread_mutex_lock(&(queue->mutex)); 
	Node *temp =NULL;
	
	/**************************************************
 	*     If the Queue is full  (size = capacity)
 	*     a conditional wait is issued. Makes the current thread to sleep.
 	*     yields other thread to lock the mutex
 	*     Waken up by any signal to the conditional variable
 	*
 	*     Drawback: Need CPU cycles to bring back from sleep state to active
 	*     
 	*     Alt: Spin lock may be used. Wasting few cycles is less costlier than waking up from sleep 
 	*     But in single core, it will hung , since during the spin to acquire the lock , 
 	*     CPU  will not provide the other thread to release the mutex  
	***************************************************/
	while(queue->size==0)
	{
		q_empty_wait++;
		pthread_cond_wait(&(queue->q_empty),&(queue->mutex));
	}
	{
		temp = queue->head;
		--(queue->size);

		queue->head=queue->head->next;
           
    
		if(queue->head)
			queue->head->prev=NULL;
		else
		{
		 queue->tail=NULL;
		}
	}
	if (temp)
	{
		temp->workTodo(NULL);
		free(temp);
	}
	pthread_mutex_unlock(&(queue->mutex)); 
	pthread_cond_broadcast(&(queue->q_full));
}




void * sayHello(void * msg)
{
//	printf("In hello\n");
}


void * incCount(void * msg)
{
	counter++;
//	printf("Value %d\n",counter);
}



void * decCount(void * msg)
{
	counter--;
//	printf("Value %d\n",counter);
}


void* producer(void *queue)
{
	functptr temp_func;
	for(int i=0;i<100;i++)
	{
		//sleep(0);
		switch(i%3)
		{
			case 0:
				temp_func= sayHello;
				break;
			case 1:
				temp_func= incCount;
				break;
			case 2:
				temp_func=decCount;
				break;

		}
		//printf("Enqueued1 ");
		enqueue((queue_t*)queue,temp_func);

		//printf("Enqueued2 ");
	}

}


void* consumer(void *queue)
{
	for(int i=0;i<100*producerTh/consumerTh;i++)
	{
		//sleep(3);
		//printf("Dequeued1 ");
		dequeue((queue_t*)queue);
		//printf("Dequeued2 ");
	}
}


queue_t  queue ={NULL,NULL,0,30,PTHREAD_COND_INITIALIZER,PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER};
/*queue.size=0;
  queue.capacity=30;
  queue.head=NULL;
  queue.tail=NULL;
  queue.mutex=PTHREAD_MUTEX_INITIALIZER;
  queue.q_full=PTHREAD_COND_INITIALIZER;
  queue.q_empty=PTHREAD_COND_INITIALIZER;*/
int main()
{
	pthread_t pthd[producerTh];
	pthread_t pthd1[consumerTh];

	for(int i=0;i<producerTh;i++)
		pthread_create(&pthd[i],NULL,producer,&queue);
	for(int i=0;i<consumerTh;i++)
		pthread_create(&pthd1[i],NULL,consumer,&queue);

	for(int i=0;i<producerTh;i++)
		pthread_join(pthd[i],NULL);
	for(int i=0;i<consumerTh;i++)
		pthread_join(pthd1[i],NULL);

	for(int i=0;i<producerTh;i++)
		pthread_exit(&pthd[i]);
	for(int i=0;i<consumerTh;i++)
		pthread_exit(&pthd1[i]);


	return 0;

}

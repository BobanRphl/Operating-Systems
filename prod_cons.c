#include "prod_cons.h"

struct Monitor* create_monitor(int size)
{
  struct Monitor* monitor=(struct Monitor*) malloc(sizeof(struct Monitor));
  monitor->buffer_size=size;
  monitor->elements=0;
  monitor->front=-1;
  monitor->rear=-1;
  monitor->buffer=(int*) malloc(monitor->buffer_size * sizeof(int));
  pthread_mutex_init(&(monitor->lock),NULL);
  pthread_cond_init(&(monitor->full),NULL);
  pthread_cond_init(&(monitor->empty),NULL);
  return monitor;
}


bool is_full(struct Monitor* monitor)
{
  if (monitor->rear == (monitor->front - 1))
	  return true;
  else
	  return false;
}

bool is_empty(struct Monitor* monitor)
{
  if(monitor->elements==0)
	  return true;
  else
	  return false;
}

void enqueue(struct Monitor* monitor, int item)
{
  monitor->rear=(monitor->rear+1)%monitor->buffer_size;
  monitor->buffer[monitor->rear]=item;
  monitor->elements++;
  //printf("front=%d rear=%d\n",monitor->front,monitor->rear);
  // printf("%d enqueued to position %d in the buffer\n",item,monitor->rear);

}

int dequeue(struct Monitor* monitor)
{
  //if (is_empty(monitor))
  //return;
  int item=monitor->buffer[monitor->front];
  monitor->front=(monitor->front+1)%monitor->buffer_size;
  monitor->elements--;
  return item;
}

void* producer(void* threadarg)
{
  struct threadargs* producerargs=(struct threadargs*) threadarg;
  struct Monitor* producer_monitor;
  producer_monitor= producerargs->monitor;
  int number_values=(producer_monitor->buffer_size)*2;
  printf("P%d: Producing %d values.\n",producerargs->threadnumber,number_values);
  for (int i=0;i<number_values;i++)
    {
      pthread_mutex_lock(&(producer_monitor->lock));
      int data=rand()%20;
      while (is_full(producer_monitor) == true)
	{
	  printf("P%d: Blocking due to full buffer\n",producerargs->threadnumber);
	  pthread_cond_wait(&(producer_monitor->empty),&(producer_monitor->lock));
	}
      enqueue(producer_monitor,data);
      printf("P%d: Writing %d to position %d\n",producerargs->threadnumber,data,producer_monitor->rear);
      //printf("P%d: %d written %d to go\n",producerargs->threadnumber,i+1,(number_values-1)-i);
      pthread_cond_broadcast(&(producer_monitor->full));
      pthread_mutex_unlock(&(producer_monitor->lock));
      
    }
  printf("P%d: Completed writing all values!!\n\n\n",producerargs->threadnumber);
  pthread_exit(NULL);
}

void* consumer(void* threadarg)
{
  struct threadargs* consumerargs=(struct threadargs*) threadarg;
  struct Monitor* consumer_monitor;
  printf("C%d: Consuming %d values\n",consumerargs->threadnumber,consumerargs->consumption);
  consumer_monitor=consumerargs->monitor;
  for(int i=0;i<consumerargs->consumption;i++)
    {
      pthread_mutex_lock(&(consumer_monitor->lock));
      while (is_empty(consumer_monitor) == true)
	{
	  printf("C%d: Blocked due to empty buffer\n",consumerargs->threadnumber);
	  pthread_cond_wait(&(consumer_monitor->full),&(consumer_monitor->lock));
	}
      int data=dequeue(consumer_monitor);
      printf("C%d: Reading %d from position %d\n",consumerargs->threadnumber,data,consumer_monitor->front);
      //printf("C%d: %d consumed %d to go\n",consumerargs->threadnumber,i+1,(consumerargs->consumption)-i-1);
      pthread_cond_signal(&(consumer_monitor->empty));
      pthread_mutex_unlock(&(consumer_monitor->lock));
      
    }
  printf("C%d: Completed consuming all values!!\n\n\n",consumerargs->threadnumber);
  pthread_exit(NULL);
}

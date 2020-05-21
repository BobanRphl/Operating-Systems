/***********************************************
Main program to run producer-consumer access
***********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "prod_cons.h"

/*
int* consumption(int total_values,int consumers)
{
  static int array[consumers];
  int multiplier=total_values/consumers;
  for (int i=0;i<(consumers-1);i++)
    {
      array[i]=multiplier;
    }
  array[consumers]=multiplier+(total_values%consumers);
  return array;
}
*/


void main(int argc, char* argv[])
{
  int p,c,t;
	
  //	variable to store status of pthread_join()
  void* status;
  int buffer_size = atoi(argv[1]);
  int total_threads=atoi(argv[2])+atoi(argv[3]);
  int producer_count=atoi(argv[2]),consumer_count=atoi(argv[3]);
  pthread_t t1[producer_count], t2[consumer_count];
  int total_values=(producer_count*buffer_size*2);
  int consumption_array[consumer_count];
  int multiplier=total_values/consumer_count;
  for (int i=0;i<(consumer_count-1);i++)
    {
     consumption_array[i]=multiplier;
    }
  consumption_array[consumer_count-1]=multiplier+(total_values%consumer_count);
  
  struct Monitor* monitor;
  // 	function call to create a monitor
  monitor = create_monitor(buffer_size);
  
  struct threadargs threadpoint[total_threads];
 
  // 	array of producer threads and consumer threads
  	
  for (int i = 0; i < producer_count; i++)
    {
      threadpoint[i].monitor=monitor;
      threadpoint[i].threadnumber=i;
      threadpoint[i].producercount=producer_count;
      threadpoint[i].consumercount=consumer_count;
      threadpoint[i].consumption=consumption_array[i];

		
      printf("MAIN: started PRODUCER %d\n", i);
      p = pthread_create(&t1[i], NULL, producer, (void*) &threadpoint[i]);
      if (p)
	{
	  printf("error creating producer thread %d\n",i);
	  exit(-1);
	}
    }
	
	
  for (int i = 0; i <consumer_count; i++)
    {
      threadpoint[i+producer_count].monitor=monitor;
      threadpoint[i+producer_count].threadnumber=i;
      threadpoint[i+producer_count].producercount=producer_count;
      threadpoint[i+producer_count].consumercount=consumer_count;
      threadpoint[i+producer_count].consumption=consumption_array[i];
      
		
      printf("MAIN: Started CONSUMER %d\n",i);
      c = pthread_create(&t2[i], NULL, consumer, (void*) &threadpoint[i+producer_count]);
      if (c)
	{
	  printf("error creating thread %d\n",i);
	  exit(-1);
	}

    }
	
	
  //loop to join producers
  for (int i = 0; i < producer_count; i++)
    {
      p = pthread_join(t1[i], &status);
      if (p)
	{
	  printf("ERROR- unable to join producer %d\n",i);
	  exit(-1);
	}
      printf("PRODUCER %d joined\n",i);
    }
	
	
  //loop to join consumers
  for (int i = 0; i < consumer_count; i++)
    {
      c = pthread_join(t2[i], &status);
      if (c)
	{
	  printf("ERROR- unable to join consumer %d\n",i);
	  exit(-1);
	}
      printf("CONSUMER %d joined\n",i);
    }
  
  pthread_mutex_destroy(&(monitor->lock));
  pthread_cond_destroy(&(monitor->full));
  pthread_cond_destroy(&(monitor->empty));
  printf("Main completed! Exiting\n");
  pthread_exit(NULL);
}

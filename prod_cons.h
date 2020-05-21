#include <stdio.h>
#include <stdlib.h> 
#include <limits.h> 
#include <pthread.h>
#include <stdbool.h>

struct Monitor{
  int front, rear, buffer_size,elements;
  int* buffer;
  pthread_mutex_t lock;
  pthread_cond_t full,empty;

};

struct threadargs {
  struct Monitor* monitor;
  int threadnumber;
  int producercount;
  int consumercount;
  int consumption;
};


extern void* producer(void* threadargs);
extern void* consumer(void* threadargs);
extern struct Monitor* create_monitor(int size);
bool is_full(struct Monitor* monitor);
bool is_empty(struct Monitor* monitor);
void enqueue(struct Monitor* monitor, int item);
int dequeue(struct Monitor* monitor);

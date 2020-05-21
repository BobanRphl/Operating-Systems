#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define fifo 0
#define sjf 1
#define rr 2
#define stcf 3
#define nonpreemptive 4
#define quantum 2

//store stats for each scheduling algorithm
struct algo_stats{
  float avg_wait_time;
  float avg_turnaround_time;
  int context_switches;
  char algo[50];
};


//structure to store the data related to a process
struct process_data{
  int total_processes;
  int process_no;
  int burst_time;
  int priority;
  int arrival_time;
  int wait_time;
  int turnaround_time;
};

//the structure of a node which is used as a linked list and implemented for the ready queue
struct queue_node{
  struct process_data process;
  struct queue_node* next;
};

void fifo_main (struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output);
void sjf_main(struct process_data* process_list, int interval,struct algo_stats* avg_algo_stats,FILE *output);
void rr_main(struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output);
void stcf_main(struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output);
void nonpreemptive_main(struct process_data* process_list, int interval,struct algo_stats* avg_algo_stats,FILE *output);

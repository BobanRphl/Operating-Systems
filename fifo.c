/*
Description: This is the file for the fifo algorithm. This file runs the fifo algorithm with all the processes.
This algorithm runs the processes as they arrive exactly in the same order.The waiting time and the turnaround
time is calculated for each process,and then the avg wait time and the avg turnaround time is calculated and the
structure is calculated.
Author: Amanpreet Singh Thukral (01747234)
        Shubham Tikare (01672183)
	Boban Pallathucharry (01810169)
 */
#include "sched_sim.h"
//adding a new node to the queue
struct queue_node* fifo_new_node(struct queue_node *current_node, struct process_data next_process)
{
  struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));//allocating space to the new node
  /* assign queue_node values to temp node */
  temp->process = next_process;//defining the process in the node
  current_node->next = temp;
  temp->next = NULL;
  return temp;
}

//=============================================================================================================
//clearing the memory allocated dynamically in the heap
void fifo_free_memory(struct queue_node first_node)
{
  struct queue_node* node;
  struct queue_node* temp;
  node=&first_node;
  node=node->next;
  while(node->next != NULL)
	{

	  temp = node->next;//obtain the next node before clearing the memory
      	  free(node);//free the memory
	  node=temp;
	}
  free(node);

}

//=============================================================================================================
//function to check the arrival of a process and order it in the queue
void fifo_check_process(struct queue_node* current_node,struct process_data* process_list,int time)
{
  struct queue_node* node;
  node=current_node;
  int p=current_node->process.process_no; // initially p=0
  while(node->next != NULL)//find the last node
    node = node->next;
  while (p<(current_node->process.total_processes-1))// check all the processes
    {
      if ((process_list[p+1].arrival_time)==time)//check for the arrival time
	{
	  /* printf("time=%d\nprocess %d \narrival time%d\n",time,(p+1),process_list[p+1].arrival_time); */
	  fifo_new_node(node,process_list[p+1]); // add new node to the end of last node
	}
      p++;  //
    }

}//check process
//=============================================================================================================
//function to print at the required data at the defined interval
void fifo_print_stats(struct queue_node* current_node, int rbt, int time,FILE *output)
{
  if(current_node->process.burst_time == rbt)//if the process is yet to start
    {
      fprintf(output,"\nTime=%d\nLoading process %d\t(CPU burst= %d)\n",time,current_node->process.process_no,current_node->process.burst_time);
      fprintf(output,"\nReady Queue:");
      /* printf("\nTime=%d\nLoading process %d\t(CPU burst= %d)\n",time,current_node->process.process_no,current_node->process.burst_time); */
      /* printf("\nReady Queue:"); */
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	  printf("%d-",current_node->process.process_no);
	}//while
    }
  else if(rbt==0)//if the process is finished
    {
      if(current_node->next!=NULL)
	fprintf(output,"\nTime=%d\nFinishing Process %d ; Loading Process %d (CPU Burst:%d)",time,current_node->process.process_no,current_node->next->process.process_no,current_node->next->process.burst_time);
      else if(current_node->process.process_no==(current_node->process.total_processes-1))//if it is the last process
	fprintf(output,"\nTime=%d\nFinishing last Process %d ",time,current_node->process.process_no);
      fprintf(output,"\nReady Queue:");//printing the ready queue using the linked list
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
    }//elseif
  else//if a process is currently running
    {
      fprintf(output,"\nTime=%d\nRunning Process %d (Remaining Burst time:%d)",time,current_node->process.process_no,rbt);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while

    }
}

//=============================================================================================================
//function to calculate the average stats for the algos
void fifo_average_stats(unsigned int wt[],unsigned int tt[],int process_sequences[],int switches,int n,struct algo_stats* avg_algo_stats,FILE *output)
{
  float avg_wait_time=0;
  float avg_turnaround_time=0;
  char algo[50]="FIFO";
  fprintf(output,"\nFIFO Summary (WT = wait time, TT = turnaround time):\nPID   WT   TT\n");
  for (int i=0;i<n;i++)
    {
      avg_wait_time+=wt[i];
      avg_turnaround_time+=tt[i];
      fprintf(output,"%d    %d   %d\n",i,wt[i],tt[i]);
    }
  avg_wait_time=avg_wait_time/n;
  avg_turnaround_time=avg_turnaround_time/n;
  //storing the avg stats in the structure that is shared with the main file
  avg_algo_stats[fifo].avg_wait_time=avg_wait_time;
  avg_algo_stats[fifo].avg_turnaround_time=avg_turnaround_time;
  avg_algo_stats[fifo].context_switches=switches;
  strcpy(avg_algo_stats[fifo].algo,algo);
  fprintf(output,"AVG   %f   %f\n",avg_wait_time,avg_turnaround_time);
  fprintf(output,"Process Sequence-");
  for (int i=0;i<switches;i++)
    {
      fprintf(output,"%d- ",process_sequences[i]);
    }
  fprintf(output,"\nContext Switches-%d\n",switches);
}


//=============================================================================================================

// Main code
void fifo_main (struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output)
{
  int time=0;//the global time used by all the functions
  struct queue_node first_node;// solved the segfault by defining memory and hence allocating space to it
  struct queue_node* current_node = &first_node; // the current node that is running

  //The line below is causing the segfault error
  current_node->process=process_list[0];//linking the current node to the process

  current_node->next=NULL;//the pointer to the next process in the queue
  unsigned int wt[current_node->process.total_processes];
  unsigned int tt[current_node->process.total_processes];
  int bt=current_node->process.burst_time;
  int temp,context_switches=0;
  int* process_sequence=malloc(100*sizeof(int));// creating a dynamic array to store the process sequence

  int rbt=bt;

  for (int t=0;t<=bt;t++)
    {
      fifo_check_process(current_node,process_list,time);//checking if any function following the current process has arrived
      if (time>(current_node->process.arrival_time))//checking if the current time is greater than the arrival time of the process
	{
	  rbt--;//decrementing the remaining burst time
	  /* fprintf(output,"remaining burst time-- is %d\ntime=%d",rbt,time); */

	}//arrival time check
      if(time%interval==0)
	fifo_print_stats(current_node,rbt,time,output);//function that writes the data to the file

      /* fprintf(output,"\ntime++ is %d\nrbt=%d\n",time,rbt); */

      if (rbt==0)//the process finished running
	{
	  current_node->process.turnaround_time=time-current_node->process.arrival_time;//turnaround time is the current time-arrival time
	  current_node->process.wait_time=time-current_node->process.burst_time-current_node->process.arrival_time;
	  wt[current_node->process.process_no]=current_node->process.wait_time;
	  tt[current_node->process.process_no]=current_node->process.turnaround_time;
	  fprintf(output,"\nTime=%d\nProcess %d finished\nWait time=%d\nTurnaround time=%d\n",time,current_node->process.process_no,current_node->process.wait_time,current_node->process.turnaround_time);
	  fprintf(output,"\nTime=%d\nProcess %d finished\nWait time=%d\nTurnaround time=%d\n",time,current_node->process.process_no,current_node->process.wait_time,current_node->process.turnaround_time);
	  process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	  context_switches++;//incrementing the number of context_switches
	  if (current_node->next!=NULL)//is there a process in the queue
	    {
	      //updating the current node which represents the head of the ready queue
	      current_node=current_node->next;
   	      bt=current_node->process.burst_time;
	      rbt=bt;//updating the remaining burst time variable
	    }

	  else if (current_node->process.process_no==(current_node->process.total_processes-1))
	    break;

	  t=0;
	}//rbt==0
      time++;//incrementing the time for every iteration of the loop
    }//end of for loop

  //function to calculate the average stats of the algo
  fifo_average_stats(wt,tt,process_sequence,context_switches,current_node->process.total_processes,avg_algo_stats,output);
  free(process_sequence);
  fifo_free_memory(first_node);//write a code to free all the memory by iterating through the ready queue which is a linked list
  return;
}

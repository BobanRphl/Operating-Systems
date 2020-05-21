/*
Description: The file runs the shortest time to completion first(stcf) algorithm for all the processes. This algorithm runs the 
job which has the shortest time to complete first. The job then preempts the job if the next job has burst time less than the 
remaining burst time of the current process waiting time and the turnaround time is calculated for each process,and then the avg wait time and the 
avg turnaround time is calculated and the structure is calculated. 
Author: Amanpreet Singh Thukral (01747234)
        Shubham Tikare (01672183)
	Boban Pallathucharry (01810169)
*/
#include "sched_sim.h"

//adding a new node to the queue
struct queue_node* stcf_new_node(struct queue_node *current_node, struct process_data next_process)
{
  struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
  /* assign queue_node values to temp node */
  temp->process = next_process;
  current_node->next = temp;
  temp->next = NULL;
  return temp;
}

//=============================================================================================================
//function to print the stats at the desired interval
void stcf_print_stats(struct queue_node* current_node,unsigned int rbt, int time,FILE *output)
{
 
  if(current_node->process.burst_time == rbt)//if the process is yet to start
    {
      fprintf(output,"\nTime=%d\nLoading process %d\t(CPU burst= %d)\n",time,current_node->process.process_no,current_node->process.burst_time);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
    }
  else if(rbt==0)// if the process finished
    {
      if(current_node->next!=NULL)
	fprintf(output,"\nTime=%d\nFinishing Process %d ; Loading Process %d (CPU Burst:%d)",time,current_node->process.process_no,current_node->next->process.process_no,current_node->next->process.burst_time);
      else if(current_node->process.process_no==(current_node->process.total_processes-1))
	fprintf(output,"\nTime=%d\nFinishing last Process %d ",time,current_node->process.process_no);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
    }//elseif
  else//the process is still running
    {
      fprintf(output,"\nTime=%d\nRunning Process %d (Remaining Burst time:%d)",time,current_node->process.process_no,rbt);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
      
    }//else
  fprintf(output,"\n");
}

//=============================================================================================================

struct queue_node* stcf_check_process(struct queue_node* current_node,struct process_data* process_list,int time, unsigned int rbt[])
{
  /* fprintf(output,"checking process!!!\n"); */
  struct queue_node* node;
  node=current_node;
  int p=current_node->process.process_no;
  while (p<(current_node->process.total_processes-1))//iterates through all the processes in the list
    {
      if ((process_list[p+1].arrival_time)==time)
	{
	  /* fprintf(output,"process %d arrived at time %d\n",p+1,time); */
	  if ((process_list[p+1].burst_time)<rbt[current_node->process.process_no])//if the burst time of the new process is less than the remaining burst time of the currently running process
	    {
	      /* fprintf(output,"changing the current node to process %d\n",(p+1)); */
	      struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
	      temp->process=process_list[p+1];
	      temp->next=current_node;
	      return temp;
	    }
	  else //checking if the burst time is less than the remaining burst time of the processes in the queue
	    {
	      while (node->next!=NULL)
		{
		  if ((process_list[p+1].burst_time)<rbt[node->next->process.process_no])//checking for the burst times in the queue
		    {
		      /* fprintf(output,"process %d inserted after process %d\n",p+1,node->process.process_no); */
		      struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
		      temp->process=process_list[p+1];
		      temp->next=node->next;
		      node->next=temp;
		      return current_node;
		    }
		  node=node->next;
		}//while
	      stcf_new_node(node,process_list[p+1]);
	    }//elseif
	}
      p++;
    }//while
  return current_node;
}//check process


//=============================================================================================================
//function to the print the average stats for the algo
void stcf_avg_stats(unsigned int wt[],unsigned int tt[],int process_sequences[],int switches,int n,struct algo_stats* avg_algo_stats,FILE *output)
{
  float avg_wait_time=0;
  float avg_turnaround_time=0;
  char algo[50]="STCF";
  fprintf(output,"\nSTCF Summary (WT = wait time, TT = turnaround time):\nPID   WT   TT\n");
  for (int i=0;i<n;i++)
    {
      avg_wait_time+=wt[i];
      avg_turnaround_time+=tt[i];
      fprintf(output,"%d    %d   %d\n",i,wt[i],tt[i]);
    }
  avg_wait_time=avg_wait_time/n;
  avg_turnaround_time=avg_turnaround_time/n;
  //average stats for the algo stored in the structure
  avg_algo_stats[stcf].avg_wait_time=avg_wait_time;
  avg_algo_stats[stcf].avg_turnaround_time=avg_turnaround_time;
  avg_algo_stats[stcf].context_switches=switches;
  strcpy(avg_algo_stats[stcf].algo,algo);
  fprintf(output,"AVG   %f   %f\n",avg_wait_time,avg_turnaround_time);
  fprintf(output,"Process Sequence-");
  for (int i=0;i<switches;i++)
    {
      fprintf(output,"%d- ",process_sequences[i]);
    }
  fprintf(output,"\nContext Switches-%d\n",switches);
}

//=============================================================================================================

void stcf_main(struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output)
{
  struct queue_node first_node;
  struct queue_node* start;
  struct queue_node* current_node=&first_node;
  int time=0;
  current_node->process=process_list[0];
  current_node->next=NULL;

  int temp,context_switches=0;
  int* process_sequence=malloc(100*sizeof(int));
  //arrays to store the remaining burst times, waiting times and the turnaround times
  unsigned int rbt[current_node->process.total_processes];
  unsigned int wt[current_node->process.total_processes];
  unsigned int tt[current_node->process.total_processes];
  //creating the remaining burst time arrays
  for (int i=0;i<current_node->process.total_processes;i++)
    {
      rbt[i]=process_list[i].burst_time;
      /* fprintf(output,"burst time for process %d is %d\n",i,rbt[i]); */
    }//rbt for loop

  for (int t=0;;t++)
    {
      
      if (time>(current_node->process.arrival_time))
	{
	  rbt[current_node->process.process_no]--;
	  /* fprintf(output,"\ncycle=%d\nremaining burst time for process %d is %d\n",time,current_node->process.process_no,rbt[current_node->process.process_no]); */
	}//arrival time check

      if(rbt[current_node->process.process_no]==0 )
	{
	  if(current_node->process.process_no!=process_sequence[context_switches-1])
	    {
	      process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	      context_switches++;//incrementing the number of context_switches
	    }
	  current_node->process.wait_time=time-current_node->process.burst_time-current_node->process.arrival_time;
	  current_node->process.turnaround_time=time-current_node->process.arrival_time;//arrival time is the current time-arrival time
	  wt[current_node->process.process_no]=current_node->process.wait_time;
	  tt[current_node->process.process_no]=current_node->process.turnaround_time;
	  fprintf(output,"\nTime=%d\nProcess %d finished\nWait time=%d\nTurnaround time=%d\n",time,current_node->process.process_no,current_node->process.wait_time,current_node->process.turnaround_time);
	  
	  if (current_node->next!=NULL)
	    {
	      current_node=current_node->next;
	      t=0;
	    }
	  else
	    break;
	  fprintf(output,"next process in the queue is %d",current_node->process.process_no);

	}//rbt check
      if (time%interval==0)
	stcf_print_stats(current_node,rbt[current_node->process.process_no],time,output);
      current_node=stcf_check_process(current_node,process_list,time,rbt);
      if(current_node->process.process_no!=process_sequence[context_switches-1])
	{
	  process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	  context_switches++;//incrementing the number of context_switches
	}
      time++;
      
    }//for
  stcf_avg_stats(wt,tt,process_sequence,context_switches,current_node->process.total_processes,avg_algo_stats,output);
  free(process_sequence);
}//main

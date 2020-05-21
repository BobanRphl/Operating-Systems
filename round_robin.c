
/*
Description: The file runs the round robin(rr) algorithm for all the processes. This algorithm runs the
job for fixed quantum. The job then shifts to the next process and the next job runs for the same quantum.
The waiting time and the turnaround time is calculated for each process,and then the avg wait time and the
avg turnaround time is calculated and the structure is calculated.
Author: Amanpreet Singh Thukral (01747234)
        Shubham Tikare (01672183)
	Boban Pallathucharry (01810169)
 */
#include "sched_sim.h"

struct queue_node* rr_new_node(struct queue_node *current_node, struct process_data next_process)
{
  struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
  /* assign queue_node values to temp node */
  temp->process = next_process;
  current_node->next = temp;
  temp->next = NULL;
  return temp;
}

//=====================================================================================================

void rr_free_memory(struct queue_node first_node)
{
  struct queue_node* node;
  struct queue_node* temp;
  node=&first_node;
  node=node->next;
  while(node->next != NULL)
	{

	  temp = node->next;
      	  free(node);
	  node=temp;
	}
  free(node);

}


//=============================================================================================================
//function to print the stats
void rr_print_stats(struct queue_node* current_node,unsigned int rbt, int time,FILE *output)
{
  if(current_node->process.burst_time == rbt)//yet to start the current process
    {
      fprintf(output,"\nTime=%d\nLoading process %d\t(CPU burst= %d)\n",time,current_node->process.process_no,current_node->process.burst_time);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
    }
  else if(rbt==0)//the process is finished
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
  else
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
//funcion to print the average stats
void rr_avg_stats(unsigned int wt[],unsigned int tt[],int process_sequences[],int switches,int n,struct algo_stats* avg_algo_stats,FILE *output)
{
  float avg_wait_time=0;
  float avg_turnaround_time=0;
  char algo[50]="Round Robin";
  fprintf(output,"\nRound Robin Summary (WT = wait time, TT = turnaround time):\nPID   WT   TT\n");
  for (int i=0;i<n;i++)
    {
      avg_wait_time+=wt[i];
      avg_turnaround_time+=tt[i];
      fprintf(output,"%d    %d   %d\n",i,wt[i],tt[i]);
    }
  avg_wait_time=avg_wait_time/n;//average wit time for the algo
  avg_turnaround_time=avg_turnaround_time/n;//average turnaround time for the algo
  avg_algo_stats[rr].avg_wait_time=avg_wait_time;
  avg_algo_stats[rr].avg_turnaround_time=avg_turnaround_time;
  avg_algo_stats[rr].context_switches=switches;
  strcpy(avg_algo_stats[rr].algo,algo);
  fprintf(output,"AVG   %f   %f\n",avg_wait_time,avg_turnaround_time);
  fprintf(output,"Process Sequence-");
  for (int i=0;i<switches;i++)
    {
      fprintf(output,"%d- ",process_sequences[i]);
    }
  fprintf(output,"\nContext Switches-%d\n",switches);
}

//=============================================================================================================

void rr_check_process(struct queue_node* current_node,struct process_data* process_list,int time,unsigned int rbt[])
{
  struct queue_node* last=current_node;
  struct queue_node* node;
  node=current_node;
  int p=current_node->process.process_no;
  while (p<(current_node->process.total_processes-1))
    {
      if ((process_list[p+1].arrival_time)==time)//if the process is yet to start
	{
	  if(node->next!=NULL)//if there is a next node
	    {
	      while (node->next!=NULL && (rbt[node->next->process.process_no]<node->next->process.burst_time))//if the process is not run even once
		node=node->next;
	      if(node->next!=NULL)//checking for which process arrived first
		if ((process_list[p+1].arrival_time)>node->next->process.arrival_time)
		  node=node->next;
	      struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
	      temp->process=process_list[p+1];
	      temp->next=node->next;
	      node->next=temp;

	    }
	  else //putting the process in the last node
	    {
	      while (node->next!=NULL)
		node=node->next;
	      rr_new_node(node,process_list[p+1]);
	    }


	}//arrival
      p++;
    }//while

}//check process


//=============================================================================================================

void rr_main(struct process_data* process_list,int interval,struct algo_stats* avg_algo_stats,FILE *output)
{
  struct queue_node first_node;
  struct queue_node* current_node=&first_node;//the current node which is the first process that starts
  int time=0;
  current_node->process=process_list[0];//setting the first process in the list to start running
  current_node->next=NULL;//no process in the queue

  int temp,context_switches=0;
  int* process_sequence=malloc(150*sizeof(int));
  int q=0;
  //arrays to store the remaning burst time, the avg wait time and the avg turnaround time
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
	  q++;//keeping a check on the cycles the process has run for
	}//arrival time check

      if(rbt[current_node->process.process_no]==0 )//if the process has finished
	{
	  if(process_sequence[context_switches-1]!=current_node->process.process_no)//checking if there is a context switch
	    {
	      process_sequence[context_switches]=current_node->process.process_no;
	      context_switches++;
	    }
	  current_node->process.wait_time=time-current_node->process.burst_time-current_node->process.arrival_time;
	  current_node->process.turnaround_time=time-current_node->process.arrival_time;//turnaround time is the current time-arrival time
	  wt[current_node->process.process_no]=current_node->process.wait_time;
	  tt[current_node->process.process_no]=current_node->process.turnaround_time;
	  fprintf(output,"\nTime=%d\nProcess %d finished\nWait time=%d\nTurnaround time=%d\n",time,current_node->process.process_no,current_node->process.wait_time,current_node->process.turnaround_time);
	  q=0;//reseting the quantum
	  if (current_node->next!=NULL)//updating the next process from the queue
	    {
	      current_node=current_node->next;
	      t=0;
	    }
	  else
	    break;
	  fprintf(output,"next process in the queue is %d",current_node->process.process_no);

	}//rbt check

      rr_check_process(current_node,process_list,time,rbt);
      if (time%interval==0)
	rr_print_stats(current_node,rbt[current_node->process.process_no],time,output);
      if (q==quantum&&time!=0)//change the process since it has run for the quantum decided
	{
	  process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	  context_switches++;//incrementing the number of context_switches
	  q=0;
	  if (rbt[current_node->process.process_no]>0)//checking if the process has some burst time remaining
	    {
	      struct queue_node* node;
	      node=current_node;
	      while(node->next != NULL)//putting it in the end of the queue
		node = node->next;
	      rr_new_node(node,process_list[current_node->process.process_no]);
	    }
	  if (current_node->next!=NULL)//checking if there is any process in queue
	    {
	      current_node=current_node->next;
	      t=0;//reset the loop
	    }
	}//t= quantum


      time++;
    }//for loop
  rr_avg_stats(wt,tt,process_sequence,context_switches,current_node->process.total_processes,avg_algo_stats,output);
  free(process_sequence);
  rr_free_memory(first_node);//write a code to free all the memory by iterating through the ready queue which is a linked list
  return;
}//main

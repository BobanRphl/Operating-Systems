/*
Description: This algorothm runs the processes with the highest priority (lowest priority number)first. On arrival, the 
processes are checked for the priority. If a process with higher priority arrives, the ready queue is checked and the 
queue is rearranged to fit the process with higher priority first . The turnaround time is calculated for each process,
and then the avg wait time and the avg turnaround time is calculated and the structure is calculated.
Author: Amanpreet Singh Thukral (01747234)
        Shubham Tikare (01672183)
	Boban Pallathucharry (01810169)
*/
#include "sched_sim.h"

//function to add a new node 
struct queue_node* np_new_node(struct queue_node *current_node, struct process_data next_process)
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
void np_print_stats(struct queue_node* current_node,unsigned int rbt, int time,FILE *output)
{
 
  if(current_node->process.burst_time == rbt)//if the process is yet to start
    {
      fprintf(output,"\nTime=%d\nLoading process %d\t(CPU burst= %d)\n",time,current_node->process.process_no,current_node->process.burst_time);
      fprintf(output,"\nReady Queue:");
      while(current_node->next!=NULL)
	{
	  /* if (current_node->next!=NULL) */
	  /* fprintf(output,"changing the current process %d\n",current_node->next->process.process_no); */
	  current_node=current_node->next;
	  fprintf(output,"%d-",current_node->process.process_no);
	}//while
    }
  else if(rbt==0)//if the process has finished 
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
  else//if the process is still running 
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
//function to print the avg stats for the algo
void np_avg_stats(unsigned int wt[],unsigned int tt[],int process_sequences[],int switches,int n,struct algo_stats* avg_algo_stats,FILE *output)
{
  float avg_wait_time=0;
  float avg_turnaround_time=0;
  char algo[50]="Nonpreemptive";
  fprintf(output,"\nNon Preemptive Summary (WT = wait time, TT = turnaround time):\nPID   WT   TT\n");
  for (int i=0;i<n;i++)
    {
      avg_wait_time+=wt[i];
      avg_turnaround_time+=tt[i];
      fprintf(output,"%d    %d   %d\n",i,wt[i],tt[i]);
    }
  avg_wait_time=avg_wait_time/n;
  avg_turnaround_time=avg_turnaround_time/n;
  //storing the avg wait time and turnaround time in the structure
  avg_algo_stats[nonpreemptive].avg_wait_time=avg_wait_time;
  avg_algo_stats[nonpreemptive].avg_turnaround_time=avg_turnaround_time;
  avg_algo_stats[nonpreemptive].context_switches=switches;
  strcpy(avg_algo_stats[nonpreemptive].algo,algo);
  fprintf(output,"AVG   %f   %f\n",avg_wait_time,avg_turnaround_time);
  fprintf(output,"Process Sequence-");
  for (int i=0;i<switches;i++)
    {
      fprintf(output,"%d- ",process_sequences[i]);
    }
  fprintf(output,"\nContext Switches-%d\n",switches);
}

//=============================================================================================================

struct queue_node* np_check_process(struct queue_node* current_node,struct process_data* process_list,int time,unsigned int rbt[])
{
  struct queue_node* node;
  node=current_node;
  int p=current_node->process.process_no;
  while (p<(current_node->process.total_processes-1))//iterates through all the processes in the list
    {
      if ((process_list[p+1].arrival_time==time))//checking for the arrival time
	{
	  if(rbt[current_node->process.process_no]==current_node->process.burst_time)
	    {
	      if(process_list[p+1].priority<current_node->process.priority)//if the next process is higher priority
		{
		  struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
		  temp->process=process_list[p+1];
		  temp->next=current_node;
		  return temp;
		}//priority check
	      else
		{
		  while (node->next!=NULL)
		    {
		      if ((process_list[p+1].priority)<node->next->process.priority)//checking if the priority is higher than the next node
			{
			  struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
			  temp->process=process_list[p+1];
			  temp->next=node->next;
			  node->next=temp;
			  return current_node;
			}//priority check for the next node
		      node=node->next;
		    }//while for last node
		  np_new_node(node,process_list[p+1]);
		  return current_node;
		}//else for priority
	    }//checking if the process has started or not
	  else if(rbt[current_node->process.process_no]<current_node->process.burst_time)
	    {
	      while (node->next!=NULL)
		{
		  if ((process_list[p+1].priority)<node->next->process.priority)//checking if the priority is higher than the next node
		    {
		      struct queue_node *temp = (struct queue_node*) malloc(sizeof(struct queue_node ));
		      temp->process=process_list[p+1];
		      temp->next=node->next;
		      node->next=temp;
		      return current_node;
		    }//priority check for the next node
		  node=node->next;
		}//while for last node
	      np_new_node(node,process_list[p+1]);
	    }//else process already started
	}//arrival
	p++;
    }//while
    
      return current_node;
}//check process

//=============================================================================================================

void nonpreemptive_main(struct process_data* process_list, int interval,struct algo_stats* avg_algo_stats,FILE *output)
{
  struct queue_node first_node;//allocating space for the first node in the queue
  struct queue_node* current_node=&first_node;

  int time=0;//the variable which will act as a clock
  current_node->process=process_list[0];//starting with process 0 and then checking if any process arrives at the same time
  current_node->next=NULL;

  int temp,context_switches=0;
  int* process_sequence=malloc(100*sizeof(int));
  //arrays to store the remaining burst time, wait time and the turnaround time for the process
  unsigned int rbt[current_node->process.total_processes];
  unsigned int wt[current_node->process.total_processes];
  unsigned int tt[current_node->process.total_processes];
  //creating th remaining burst time arrays
  for (int i=0;i<current_node->process.total_processes;i++)
    {
      rbt[i]=process_list[i].burst_time;
      /* fprintf(output,"burst time for process %d is %d\n",i,rbt[i]); */
    }//rbt for loop
  

  for(int t=0;;t++)
    {
      
      if (time>(current_node->process.arrival_time))
	{
	  rbt[current_node->process.process_no]--;
	  /* fprintf(output,"\nremaining burst time is %d\n",rbt[current_node->process.process_no]); */
	}//arrival time check
      
      if(rbt[current_node->process.process_no]==0 )//if the process has finished
	{
	  if (context_switches==0)//if it was the first process
	    {
	      process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	      context_switches++;//incrementing the number of context_switches
	    }
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
	np_print_stats(current_node,rbt[current_node->process.process_no],time,output);
      current_node=np_check_process(current_node,process_list,time,rbt);
      if(current_node->process.process_no!=process_sequence[context_switches-1])
	{
	  process_sequence[context_switches]=current_node->process.process_no;//adding to the process sequence array if the current_node changes
	  context_switches++;//incrementing the number of context_switches
	}
      time++;
    }//for
  np_avg_stats(wt,tt,process_sequence,context_switches,current_node->process.total_processes,avg_algo_stats,output);
  free(process_sequence);
}//main

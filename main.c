/*
Description: This is the main file to run all the required algorithms. The file accepts the command line arguments
and parses the data to extract the paameters for the process. Then it calls each of the algorithms and prints the
sorted values.
Author: Amanpreet Singh Thukral (01747234)
        Shubham Tikare (01672183)
	Boban Pallathucharry (01810169)
 */
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sched_sim.h"

//the function to swap the positions of the structures
void swap(struct algo_stats* xp,struct algo_stats* yp)
{
    struct algo_stats temp = *xp;
    *xp = *yp;
    *yp = temp;
}

//function to sort the average wait times of all the algos
void sort_wait_time(struct algo_stats arr[])
{
  int n=5;
  int i, j;
  for (i = 0; i < n-1; i++)

    // Last i elements are already in place
    for (j = 0; j < n-i-1; j++)
      if (arr[j].avg_wait_time > arr[j+1].avg_wait_time)
	swap(&arr[j], &arr[j+1]);
}
//printing the sorted wait time array
void print_wait_time(struct algo_stats arr[],FILE *output)
{
  fprintf(output,"Wait Time Comparison\n");
  int i;
  for (i=0; i < 5; i++)
    fprintf(output,"%d.\t%s\t%f\n",i+1,arr[i].algo, arr[i].avg_wait_time);
}

//==============================================================================================
//function to sort the average turnaround time
void sort_turnaround_time(struct algo_stats arr[])
{
  int n=5;
  int i, j;
  for (i = 0; i < n-1; i++)

    // Last i elements are already in place
    for (j = 0; j < n-i-1; j++)
      if (arr[j].avg_turnaround_time > arr[j+1].avg_turnaround_time)
	swap(&arr[j], &arr[j+1]);
}

//function to print the turnaroud time for all the algos
void print_turnaround_time(struct algo_stats arr[],FILE *output)
{
  fprintf(output,"Turnaround Time Comparison\n");
  int i;
  for (i=0; i < 5; i++)
    fprintf(output,"%d.\t%s\t%f\n",i+1,arr[i].algo, arr[i].avg_turnaround_time);
}

//==============================================================================================
//sorting the context switches for all the algos
void sort_context_switch(struct algo_stats arr[])
{
  int n=5;
  int i, j;
  for (i = 0; i < n-1; i++)

    // Last i elements are already in place
    for (j = 0; j < n-i-1; j++)
      if (arr[j].context_switches > arr[j+1].context_switches)
	swap(&arr[j], &arr[j+1]);
}
//printing the context switched for the algos
void print_context_switch(struct algo_stats arr[],FILE *output)
{
  fprintf(output,"Context Switch Comparison\n");
  int i;
  for (i=0; i < 5; i++)
    fprintf(output,"%d.\t%s\t%d\n",i+1,arr[i].algo, arr[i].context_switches);
}

//==============================================================================================
//function to parse the .dat file to extract the data for each process
struct process_data* parse_data(FILE *fp)
/* void parse_data(FILE *fp) */
{
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  char *token;
  int* data=malloc(100 * sizeof(int)); //variable input filesize
  int count=0;
  int number_processes=0;
  //reads the file line by line to retrieve the data of each process
  while ((read = getline(&line, &len, fp)) != -1)
    {
      /* printf("%s\n", line); */
      /* get the first token */
      token = strtok(line,"  ");//token splits the string passed in accordance with the delimeter which is the 2nd argument
      while(token != NULL)
	{
	  data[count]=strtol(token,NULL,10); // converting the string to integer
	  token = strtok(NULL, "  ");        //
	  count++;
	}
    }

  number_processes=count/3;//calculating the number of processes in the .dat file
  struct process_data* process=malloc(number_processes*(sizeof(struct process_data)));//allocating the space to store the data related to the processes
  /* printf("number of processes %d \n",number_processes); */
  //storing the data extracted to the structures
  for (int i=0;i<number_processes;i++)
    {
      process[i].total_processes=number_processes;
      process[i].process_no=i;
      process[i].burst_time=data[(i*3)];
      process[i].priority=data[(i*3)+1];
      process[i].arrival_time=data[(i*3)+2];
      process[i].wait_time=0;
      process[i].turnaround_time=0;
    }

  fclose(fp);//close the .dat file
  free(data);//freeing the memory allocated for the data array
  data=NULL;
  return process;//returning the pointer to the start of the array that stores the structures of the processes
}
//==============================================================================================
static int interval;
void main(int argc, char *argv[])
{
  char filename[20],output_file_name[50];
  sprintf (filename,"%s",argv[1]);
  sprintf(output_file_name,"%s",argv[2]);
  FILE *fp;
  FILE *output;
  interval=atoi(argv[3]);
  //open the file  and read the data
  fp = fopen(filename, "r");
  output=fopen(output_file_name,"w");
  struct process_data* process_list=parse_data(fp); //struct process_data* process  returned from the function
  struct algo_stats avg_algo_stats[5];//structures to store the avg stats related to the algos

  //calling the algorithm functions
  fprintf(output,"\n*****************FIFO SCHEDULING********************\n");
  fifo_main(process_list,interval,avg_algo_stats,output);
  fprintf(output,"\n*****************SJF SCHEDULING********************\n");
  sjf_main(process_list,interval,avg_algo_stats,output);
  fprintf(output,"\n*****************ROUND ROBIN SCHEDULING********************\n");
  rr_main(process_list,interval,avg_algo_stats,output);
  fprintf(output,"\n*****************STCF SCHEDULING********************\n");
  stcf_main(process_list,interval,avg_algo_stats,output);
  fprintf(output,"\n*****************NONPREEMPTIVE SCHEDULING********************\n");
  nonpreemptive_main(process_list,interval,avg_algo_stats,output);

  fprintf(output,"***** OVERALL SUMMARY *****\n");
  //calling the functions to sort and print the overall stats related to each algo
  sort_wait_time(avg_algo_stats);
  print_wait_time(avg_algo_stats,output);
  sort_turnaround_time(avg_algo_stats);
  print_turnaround_time(avg_algo_stats,output);
  sort_context_switch(avg_algo_stats);
  print_context_switch(avg_algo_stats,output);
  free(process_list);  // segmentation fault

  process_list=NULL;  // dangling pointer
  return ;
}

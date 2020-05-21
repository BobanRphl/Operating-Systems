/*********************************
EECE.5730 OPERATING SYSTEMS
Instructor: Dr. Michael J. Geiger
Submitted by: Boban R. Pallathucharry
Student ID- 01810169
**********************************/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>


int main(int argc, char *argv[]) {
	int x=0,i=0,k=0,t=0;
	pid_t pid[25], pid2, processid1, processid2,parent;
	//parent = getpid();
	if((t = atoi(argv[1])) > 25) { 								//argv[0] is the filename itself
		printf("\n\nNumber of processes {%d} should be less than 25\n\n",t);
		exit(0);
	}
	else {
		printf("\nParent process is %d\n",parent = getpid());
		for(i=0;i<t;i++) {
			if (parent == getpid()) {
				pid2 = fork();
				pid[i] = pid2;
				if (pid2 == 0) {
					printf("\nexecuting Child %d (PID %d)\n",i+1,getpid());
					if ((i+1) % 5 == 0)
						execlp("./test1", "", NULL);
					else if ((i+1) % 5 == 1)
						execlp("./test2", "", NULL);
					else if ((i+1) % 5 == 2)
						execlp("./test3", "", NULL);
					else if ((i+1) % 5 == 3)
						execlp("./test4", "", NULL);
					else execlp("./test5", "", NULL);
				}
				else if (pid2 > 0)
					continue;
			}
		}
		for (i=0;i<t;i++) {
			x = wait(NULL);
			for (k = 0;k < t;k++) {
				if (x == pid[k])
					printf("\nChild %d (PID %d) complete\n",k+1,x);
				else
					continue;
			}
		}
	}
	return 0;
}

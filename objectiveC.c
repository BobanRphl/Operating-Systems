#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


//int process[11] = {};
int x=0,i=0,k=0,t=0;
pid_t pid[25], pid2, processid1, processid2,parent;
//pid = getpid();
int main(int argc, char *argv[]) {
	//parent = getpid();
	if((t = atoi(argv[1])) > 25) {
		printf("\n\nNumber of processes {%d} should be less than 25\n\n",t);
		exit(0);
	}
	else {
		printf("Parent process is %d\n",parent = getpid());
		for(i=0;i<t;i++) {
			if (parent == getpid()) {
				pid2 = fork();
				pid[i] = pid2;
				if (pid2 == 0) {
					printf("executing Child %d (PID %d)\n",i,getpid());
					execlp("/mnt/g/os/proj-1", "test1.c", NULL);
					//exit(0);
				}
				else if (pid2 > 0)
					continue;
			}		
		}
		for (i=0;i<t;i++) {
			x = wait(NULL);
			for (k = 0;k < t;k++) {
				if (x == pid[k])
					printf("Child %d (PID %d) complete\n",k,x);
			}
		}
	}
}
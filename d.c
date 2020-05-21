#include <sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>





void finish( int id)
{

int j,a=10;
for(j=0;j<10;j++)
{
	
printf(" \n  child: %d, has completed (pid%d)",a, id);


a--;
id= id-1;

}	
}



int main(int argc, char *argv[])
{
int children = atoi(argv[1]);
	pid_t pid[children];
if (children > 25)
	{
		printf("No. of child processes must be less than 25. Exiting program! \n \n");
		exit(1);
	}
else
{

int i=0, c=1,j,status,b;

pid_t cpid;
cpid = fork();

if(cpid < 0)
{
printf("\n Fork failed");
}
else if (cpid != 0)
{
printf("\n Parent %d ", getpid());

}

else  
{
pid_t pid;
pid = getpid();

for (i=0; i<10; i++)
{
printf("\n Child %d PID %d", c, pid);
int childindex = i+1;
if(childindex%5==1)
			{
				execlp("./test1","ls", NULL);
			}
			else if(childindex%5==2)
			{
				execlp("./test2","ls", NULL);
			}
			else if(childindex%5==3)
			{
				execlp("./test3","ls", NULL);
			}
			else if(childindex%5==4)
			{
				execlp("./test4","ls", NULL);
			}
			
			else
			{
				execlp("./test5","ls", NULL);
			}
			exit(0);
c++;
pid =pid+1;
}
b=pid-1;

finish (b);

}
return 0;


}
}
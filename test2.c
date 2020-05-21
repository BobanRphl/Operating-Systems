//
//  test2.c
//  
//
//  Created by Michael Geiger on 4/13/17.
//
//

#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main() {
	printf("Running program test2 in process %d\n", (int)getpid());
	printf("T2: sqrt of PID %d is %.2lf\n", (int)getpid(),
		   sqrt((double)getpid()));
	return 0;
}

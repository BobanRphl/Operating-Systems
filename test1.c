//
//  test1.c
//
//
//  Created by Michael Geiger on 4/13/17.
//  Simple program to be run in OS Project 1
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  int i;
  printf("Running program test1 in process %d\n", (int)getpid());
  for (i = 0; i < 5; i++)
    printf("T1: i %d, i^2 %d\n", i, i*i);
  return 0;
}

//
//  test4.c
//
//
//  Created by Michael Geiger on 4/13/17.
//  Simple program to be run in OS Project 1
//

#include <stdio.h>
#include <unistd.h>

int main() {
  int x = (int)getpid();
  int n = 0;
  printf("Running program test4 in process %d\n", x);
  while (x > 0) {
    n++;
    x = x / 10;
  }
  printf("T4: PID %d has %d digits\n", (int)getpid(), n);
  return 0;
}

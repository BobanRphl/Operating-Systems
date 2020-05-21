//
//  test5.c
//
//
//  Created by Michael Geiger on 4/13/17.
//  Simple program to be run in OS Project 1
//

#include <stdio.h>
#include <unistd.h>

void QS(int L[], int min, int max) {
  int i, j;
  int P;
  int temp;
  printf("T5: QS L[%d-%d]\n", min, max);

  P = L[min];
  i = max;
  j = min + 1;

  while (j < i) {
    while (L[i] > P) 
      i--;
    while ((j < i) && (L[j] < P))
      j++;

    if (j == i) {
      L[min] = L[i];
      L[i] = P;
    }
    else {
      temp = L[i];
      L[i] = L[j];
      L[j] = temp;
    }
  }

  if (min + 1 < i)
    QS(L, min, i - 1);
  if (i + 1 < max)
    QS(L, i + 1, max);
}

int main() {
  int list[] = {5, 1, 3, 10, 7, 9, 4, 2, 8, 6};
  int i;

  printf("Running program test5 in process %d\n", (int)getpid());
  QS(list, 0, 9);

  printf("T5: Final list = ");
  for (i = 0; i < 10; i++)
    printf("%d ", list[i]);
  printf("\n");
  return 0;
}

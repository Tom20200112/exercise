#include <assert.h>
#include <stdio.h>

// 1 + 1/1! + 1/2! + ... + 1/n!
// n = [1, ...]
int main(void) {
  int n;
  scanf("%d", &n);
  if (n <= 0) {
    printf("n must > 0.\n");
    return 1;
  }

  float sum = 1;
  float fac = 1;
  for (int i = 1; i <= n; i++) {
    fac *= i;
    sum += 1 / fac;
  }

  printf("%f\n", sum);

  return 0;
}
#include <assert.h>
#include <stdio.h>

#define countof(x) (sizeof(x) / sizeof(x[0]))

void swap(int s[], int len, int x, int y) {
  assert(x >= 0 && x < len);
  assert(y >= 0 && y < len);

  int t = s[x];
  s[x] = s[y];
  s[y] = t;
}

void insert_sort(int s[], int len) {
  for (int i = 0; i < len; i++) {
    for (int j = i; j > 0; j--) {
      if (s[j] >= s[j - 1]) {
        break;
      }

      assert(s[j] < s[j - 1]);
      swap(s, len, j, j - 1);
    }
  }
}

void print_out(int data[], int len) {
  for (int i = 0; i < len; i++) {
    if (i == 0) {
      printf("%d", data[i]);
    } else {
      printf(", %d", data[i]);
    }
  }

  printf("\n");
}

void check(int data[], int len) {
  for (int i = 0; i < len - 1; i++) {
    assert(data[i] <= data[i + 1]);
  }
}

void sort_and_print(int data[], int len) {
  insert_sort(data, len);
  check(data, len);
  print_out(data, len);
}

int main(void) {
  int data1[] = {10, 7, 29, 14, 33, 55, 22, 4, 56, 234};
  int data2[] = {1, 2, 489, 0, 294, 3, 43, 5, 7, 2};
  int data3[] = {46, 128, 19, 38, 1, 4, 29, 31, 239, 6};

  sort_and_print(data1, countof(data1));
  sort_and_print(data2, countof(data2));
  sort_and_print(data3, countof(data3));

  return 0;
}
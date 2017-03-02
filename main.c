#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define MAX 1000000000
#define MAX_TIME 1.1

double timing(const char *n, int (*f)(void)) {
  clock_t t = clock();
  int max = (*f)();
  double result = (double) (clock() - t) / CLOCKS_PER_SEC;

  printf("[timing] %s (%d): %.9lf sec\n", n, max, result);
  return result;
}

static int max(int l, int r) {
  return l > r ? l : r;
}

size_t S;
int *data = NULL;

int max_a(void);
int max_b(void);
int max_c(void);
int max_d(void);
int max_e(void);

int main(int argc, char *argv[]) {
  int *values = NULL;
  double k;
  static struct { const char *n; int (*f)(void); double time; } impls[5] = {
    {"a", max_a, 0.0},
    {"b", max_b, 0.0},
    {"c", max_c, 0.0},
    {"d", max_d, 0.0},
    {"e", max_e, 0.0}
  };

  assert(argc == 3);
  S = (size_t) atoi(argv[1]);
  sscanf(argv[2], "%lf", &k);

  srand((unsigned int) time(NULL));

  values = calloc(MAX, sizeof(int));
  data = calloc(MAX, sizeof(int));

  for (size_t i = 0; i < MAX; ++i) {
    values[i] = (int) (((MAX/3) + 1) * ((double) rand() / RAND_MAX));
  }

  while (S < MAX &&
    (impls[0].time < MAX_TIME
    || impls[1].time < MAX_TIME
    || impls[2].time < MAX_TIME
    || impls[3].time < MAX_TIME
    || impls[4].time < MAX_TIME)) {
    printf("\nsize: %lu\n", S);
    for (int x = 0; x < 5; ++x) {
      memcpy(data, values, (size_t) S * sizeof(int));
      if (impls[x].time < MAX_TIME) {
        impls[x].time = timing(impls[x].n, impls[x].f);
      }
    }

    S *= k;
  }

  free(values);
  free(data);

  return EXIT_SUCCESS;
}

int max_a(void) {
  int result = data[0];

  for (int i = 1; i < S; i++) {
    result = max(result, data[i]);
  }

  return result;
}

int max_b(void) {
  void swap(int *l, int *r) {
    int t = *l;
    *l = *r;
    *r = t;
  }

  int n_min = 0;

  for (int min = 0; min < S; min++) {
    n_min = min;
    for (int i = min + 1; i < S; i++) {
      if (data[i] < data[min]) {
        n_min = i;
      }
    }
    swap(&data[min], &data[n_min]);
  }

  return data[S - 1];
}

int max_c(void) {
  void merge(int *A, int a, int *B, int b, int *C) {
    int i = 0, j = 0, k = 0;
    while (i < a && j < b) {
      if (A[i] <= B[j]) C[k++] = A[i++];
      else C[k++] = B[j++];
    }
    while (i < a) C[k++] = A[i++];
    while (j < b) C[k++] = B[j++];
  }

  void merge_sort(int *A, int n) {
    int i, *A1, *A2, n1, n2;

    if (n < 2)
      return;

    n1 = n / 2;
    n2 = n - n1;
    A1 = (int *) malloc(sizeof(int) * n1);
    A2 = (int *) malloc(sizeof(int) * n2);

    for (i = 0; i < n1; i++) {
      A1[i] = A[i];
    }
    for (i = 0; i < n2; i++) {
      A2[i] = A[i + n1];
    }
    merge_sort(A1, n1);
    merge_sort(A2, n2);
    merge(A1, n1, A2, n2, A);
    free(A1);
    free(A2);
  }

  merge_sort(data, (int) S);

  return data[S - 1];
}

int max_d(void) {
  size_t size;

  int max_r(int *l, int *r, size_t s) {
    int last;

    if (s > 1) {
      if ((s % 2) != 0) {
        last = data[s];
        s = (s - 1) / 2;
        return max(last, max(max_r(l, l + s, s), max_r(r, r + s, s)));
      }
      s /= 2;
      return max(max_r(l, l + s, s), max_r(r, r + s, s));
    }
    return max(l[0], r[0]);
  }

  size = S / 2;
  return max_r(data, data + size, size);
}

int max_e(void) {
  int result = data[0];

  for (int i = 1; i < S - 1; i += 2) {
    result = max(result, max(data[i], data[i + 1]));
  }

  return result;
}

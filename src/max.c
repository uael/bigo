#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define MAX 0x2540BE400

double benchmark(const char *n, uint8_t (*f)(void)) {
  clock_t t = clock();
  double result;

  (*f)();
  result = (double) (clock() - t) / CLOCKS_PER_SEC;
  printf("%s;%.9lf\n", n, result);
  return result;
}

static uint8_t max(uint8_t l, uint8_t r) {
  return l > r ? l : r;
}

size_t size;
uint8_t *data = NULL;

uint8_t max_a(void);
uint8_t max_b(void);
uint8_t max_c(void);
uint8_t max_d(void);
uint8_t max_e(void);

uint8_t main(uint8_t argc, char *argv[]) {
  uint8_t *values;
  double k, limit;
  static struct { const char *id; uint8_t reset, (*f)(void); double time; } impls[5] = {
    {"a", 0, max_a, 0.0},
    {"b", 1, max_b, 0.0},
    {"c", 1, max_c, 0.0},
    {"d", 0, max_d, 0.0},
    {"e", 0, max_e, 0.0}
  };

  assert(argc == 4);
  size = (size_t) atoi(argv[1]);
  sscanf(argv[2], "%lf", &k);
  sscanf(argv[3], "%lf", &limit);

  srand((unsigned int) time(NULL));

  values = malloc(MAX * sizeof(uint8_t));
  data = malloc(MAX * sizeof(uint8_t));

  for (size_t i = 0; i < MAX; ++i) {
    values[i] = (uint8_t) (((UINT8_MAX/3) + 1) * ((double) rand() / RAND_MAX));
  }

  do for (uint8_t x = 0; x < 5; ++x) {
    if (impls[x].time < limit) {
      printf("%lu;", size);
      impls[x].time = benchmark(impls[x].id, impls[x].f);
      if (impls[x].reset) {
        memcpy(data, values, size * sizeof(uint8_t));
      }
    }
  } while (
    (size *= k) < MAX && (
      impls[0].time < limit
      || impls[1].time < limit
      || impls[2].time < limit
      || impls[3].time < limit
      || impls[4].time < limit
    )
  );

  free(values);
  free(data);

  return EXIT_SUCCESS;
}

uint8_t max_a(void) {
  uint8_t result = data[0];

  for (size_t i = 1; i < size; i++) {
    result = max(result, data[i]);
  }

  return result;
}

uint8_t max_b(void) {
  size_t min = 0;
  uint8_t tmp;

  for (size_t i = 0; i < size; i++) {
    min = i;
    for (size_t j = i + 1; j < size; j++) {
      if (data[j] < data[i]) {
        min = j;
      }
    }
    /* do swap */
    tmp = data[i];
    data[i] = data[min];
    data[min] = tmp;
  }

  return data[size - 1];
}

uint8_t max_c(void) {
  void merge(uint8_t *A, size_t a, uint8_t *B, size_t b, uint8_t *C) {
    size_t i = 0, j = 0, k = 0;
    while (i < a && j < b) {
      if (A[i] <= B[j]) C[k++] = A[i++];
      else C[k++] = B[j++];
    }
    while (i < a) C[k++] = A[i++];
    while (j < b) C[k++] = B[j++];
  }

  void merge_sort(uint8_t *A, size_t n) {
    uint8_t *A1, *A2;
    size_t i, n1, n2;

    if (n < 2)
      return;

    n1 = n / 2;
    n2 = n - n1;
    A1 = (uint8_t *) malloc(sizeof(uint8_t) * n1);
    A2 = (uint8_t *) malloc(sizeof(uint8_t) * n2);

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

  merge_sort(data, size);

  return data[size - 1];
}

uint8_t max_d(void) {
  uint8_t max_r(uint8_t *l, uint8_t *r, size_t s) {
    uint8_t last;

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

  size_t s = size / 2;
  return max_r(data, data + s, s);
}

uint8_t max_e(void) {
  uint8_t result = data[0];

  for (size_t i = 1; i < size - 1; i += 2) {
    result = max(result, max(data[i], data[i + 1]));
  }

  return result;
}

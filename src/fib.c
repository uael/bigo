#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long ullong;

ullong fib(ullong n) {
  switch (n) {
    case 0:
      return 0;
    case 1:
      return 1;
    default:
      return fib(n - 1) + fib(n - 2);
  }
}

ullong fib3(ullong n) {
  ullong first = 0, second = 1, next = 0, i;

  for (i = 0; i < n; ++i) {
    if (i <= 1)
      next = i;
    else {
      next = first + second;
      first = second;
      second = next;
    }
  }
  return next;
}

ullong fib2(ullong n){
  static size_t tabsize=200;
  static ullong *tab = NULL;
  if (tab==NULL){
    tab = malloc(sizeof (ullong)*tabsize);
    memset(tab, 0, tabsize);
  }
  else if(n>200){
    tab = realloc(tab, tabsize*=2);
    memset(tab-(tabsize/2), 0, tabsize);
  }
  if (tab[n] == 0){
    tab[n] = fib3(n);
  }
  return tab[n];
}

int main() {
  ullong i;

  for (i = 1; i < 200; ++i) {
    printf("%zu\n", fib(i));
  }
  return 0;
}

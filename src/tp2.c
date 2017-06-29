#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* fonction utilisateur de comparaison fournie a qsort() */
static int compare(void const *a, void const *b) {
  /* definir des pointeurs type's et initialise's
     avec les parametres */
  int const *pa = a;
  int const *pb = b;

  /* evaluer et retourner l'etat de l'evaluation (tri croissant) */
  return *pa - *pb;
}

int main(int argc, char *argv[]) {
  int *tab;
  int n = atoi(argv[1]); //atoi: convertit le string en int

  tab = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    tab[i] = (int) ((50 + 1) * ((double) rand() / RAND_MAX));
  }

  qsort(tab, (size_t) n, sizeof(int), compare);


  free(tab); //libère mem du tableau


  return EXIT_SUCCESS;
}

int card(int *tab, int n, int element) {
  int result = 0;
  for (int i = 0; i < n; i++) {
    if (tab[i] == element) {
      result++;
    } else if (result)break;
  }
  return result>n/2;
}

bool elementMaj(int *tab, int n) {
  int prev = 0, count = 0;
  for (int i = 0; i<n; ++i) {
    if (prev != tab[i]) {
      count = 0;
    }
    if (++count > n/2) return true;
    prev = tab[i];
  }
  return false;
}

bool elementMaj2(int *tab, int n) {
  int *tab2=malloc(sizeof(int)*tab[n-1]);
  memset(tab2, 0, (size_t) tab[n-1]); // set all values to 0
  for (int i = 0; i < n; i++) {
    if(++tab2[tab[i]] > n/2) return true;
  }
  return false;
}

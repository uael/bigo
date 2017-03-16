#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  return result;
}

int card2(int *tab, int n, int element){
  int *tab2=malloc(sizeof(int)*tab[n-1]);
  memset(tab2, 0, (size_t) tab[n-1]); // set all values to 0
  for (int i = 0; i < n; i++) {
    tab2[tab[i]]++;
  }

  if (element <= tab[n - 1]) {
    return tab2[element];
  } else {
    return 0;
  }
}

int card3(int *tab, int n, int element){
  if(n==1){
    if (tab[0]==element)return 1;
    return 0;
  }
  return card3(tab, n/2, element) + card3(tab + (n/2), n - (n/2), element);
}

#ifdef OPT1

void kernel (unsigned n, const float a[n], const float b[n], float c[n][n]) {
   
  unsigned i, j;

  // Précalculer les exponentielles de a et b
  float exp_a[n], exp_b[n];
  for (i = 0; i < n; i++) {
      exp_a[i] = exp(a[i]);
      exp_b[i] = exp(b[i]);
  }

  // Initialiser le tableau c à zéro
  for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
          c[i][j] = 0.0;
      }
  }

  // Remplir le tableau c en utilisant les valeurs précalculées
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
       if (j == 0)
          c[i][0] += exp_a[i] * exp_b[j];
       else
          c[i][j] += fmin(c[i][0], exp_a[i] * exp_b[j]);
    } 
  }  
}


#elif defined OPT2

#include <string.h> // memset

/* ijk -> ikj permutation to make stride 1 the innermost loop */
void kernel (unsigned n, float a[n][n], float b[n][n], float c[n][n]) {
   int i, j, k;

   memset (c, 0, n * n * sizeof c[0][0]);

   for (i=0; i<n; i++)
      for (k=0; k<n; k++)
         for (j=0; j<n; j++)
            c[i][j] += a[i][k] * b[k][j];
}

#else

/* original */
void kernel (unsigned n, const float a[n], const float b[n], float c[n][n]) {
   
  unsigned i, j;

  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
       c[i][j] = 0.0;

  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
       if (j == 0)
          c[i][0] += exp (a[i] + b[j]);
       else
          c[i][j] += fmin (c[i][0], exp (a[i] + b[j]));
    } 
  }  
}
#endif

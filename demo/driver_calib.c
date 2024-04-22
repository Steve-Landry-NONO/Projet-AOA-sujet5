#include <stdio.h>
#include <stdlib.h> // atoi, qsort
#include <stdint.h>
#include <time.h> // nanosleep

#define NB_METAS 31

extern uint64_t rdtsc ();

// TODO: adjust for each kernel
extern void kernel (unsigned n, float a[n][n], float b[n][n], float c[n][n]);

// TODO: adjust for each kernel
static void init_array (int n, float a[n][n]) {
   int i, j;

   for (i=0; i<n; i++)
      for (j=0; j<n; j++)
         a[i][j] = (float) rand() / RAND_MAX;
}

static int cmp_uint64 (const void *a, const void *b) {
   const uint64_t va = *((uint64_t *) a);
   const uint64_t vb = *((uint64_t *) b);

   if (va < vb) return -1;
   if (va > vb) return 1;
   return 0;
}

int main (int argc, char *argv[]) {

   /* check command line arguments */
   if (argc != 3) {
      fprintf (stderr, "Usage: %s <size> <nb measures>\n", argv[0]);
      return 1;
   }

   /* get command line arguments */
   const unsigned size = atoi (argv[1]); /* problem size */
   const unsigned repm = atoi (argv[2]); /* number of repetitions during measurement */

   uint64_t (*tdiff)[NB_METAS] = malloc (repm * sizeof tdiff[0]);

   unsigned m;
   for (m=0; m<NB_METAS; m++) {
      printf ("Metarepetition %u/%d: running %u instances\n", m+1, NB_METAS, repm);

      unsigned i;

      /* allocate arrays. TODO: adjust for each kernel */
      float (*a)[size] = malloc (size * size * sizeof a[0][0]);
      float (*b)[size] = malloc (size * size * sizeof b[0][0]);
      float (*c)[size] = malloc (size * size * sizeof c[0][0]);

      /* init arrays */
      srand(0);
      init_array (size, a);
      init_array (size, b);

      // No warmup, measure individual instances
      for (i=0; i<repm; i++) {
         const uint64_t t1 = rdtsc();
         kernel (size, a, b, c);
         const uint64_t t2 = rdtsc();
         tdiff[i][m] = t2 - t1;
      }

      /* free arrays. TODO: adjust for each kernel */
      free (a);
      free (b);
      free (c);

      /* Let some time for processor to cool down and allow capturing stability via next meta-repetitions */
      const struct timespec two_seconds = { .tv_sec = 2, .tv_nsec = 0 };
      nanosleep (&two_seconds, NULL);
   }

   const uint64_t nb_inner_iters = size * size * size; // TODO adjust for each kernel
   int i;
   for (i=0; i<repm; i++) {
      printf ("Instance %u/%u\n", i+1, repm);

      qsort (tdiff[i], NB_METAS, sizeof tdiff[i][0], cmp_uint64);
      printf ("MIN %lu RDTSC-cycles (%.2f per inner-iter)\n",
              tdiff[i][0], (float) tdiff[i][0] / nb_inner_iters);
      printf ("MED %lu RDTSC-cycles (%.2f per inner-iter)\n",
              tdiff[i][m/2], (float) tdiff[i][m/2] / nb_inner_iters);
      const float stab = (tdiff[i][m/2] - tdiff[i][0]) * 100.0f / tdiff[i][0];
      if (stab >= 10)
         printf ("BAD STABILITY: %.2f %%\n", stab);
      else if (stab >= 5)
         printf ("AVERAGE STABILITY: %.2f %%\n", stab);
      else
         printf ("GOOD STABILITY: %.2f %%\n", stab);
   }

   return EXIT_SUCCESS;
}

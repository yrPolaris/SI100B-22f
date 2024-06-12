#include <immintrin.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void impl(int N, int step, double *p) {
  double *p_next = (double *)aligned_alloc(1, N * N * sizeof(double));
  double *p_p_next = p_next;
  omp_set_num_threads(28);
  __m256d set = _mm256_set1_pd(0.25);
  memcpy(p_next, p, N * N * sizeof(double));
  for (int k = 0; k < step; k++) {
#pragma omp parallel for
    for (int i = 1; i < N - 1; i++) {
      int j;
      for (j = 1; j < N - 4; j += 4) {
        __m256d above = _mm256_loadu_pd(&p[(i - 1) * N + j]);
        __m256d below = _mm256_loadu_pd(&p[(i + 1) * N + j]);
        __m256d right = _mm256_loadu_pd(&p[i * N + j + 1]);
        __m256d left = _mm256_loadu_pd(&p[i * N + j - 1]);
        __m256d result = _mm256_add_pd(_mm256_add_pd(above, below),
                                       _mm256_add_pd(right, left));
        result = _mm256_mul_pd(result, set);
        _mm256_storeu_pd(&p_next[i * N + j], result);
      }
      for (; j < N - 1; j++) {
        p_next[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
                             p[i * N + j + 1] + p[i * N + j - 1]) /
                            4.0;
      }
    }
    double *temp = p;
    p = p_next;
    p_next = temp;
    // #pragma omp parallel for
    //     for (int i = 1; i < N - 1; i++) {
    //       int j;
    //       for (j = 1; j < N - 4; j += 4) {
    //         __m256d next_vals = _mm256_loadu_pd(&p_next[i * N + j]);
    //         _mm256_storeu_pd(&p[i * N + j], next_vals);
    //       }
    //       for (; j < N - 1; j++) {
    //         p[i * N + j] = p_next[i * N + j];
    //       }
    //     }
  }

  free(p_p_next);
}
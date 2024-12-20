#include <immintrin.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

void impl(int N, int step, double *p) {
  /* Your code here */
  double *p_copy = (double *)aligned_alloc(64, N * N * sizeof(double));
  memcpy(p_copy, p, N * N * sizeof(double));
  double *p_p = p;
  p = p_copy;

  double *p_next = (double *)aligned_alloc(64, N * N * sizeof(double));
  double *p_p_next = p_next;
  memcpy(p_next, p, N * N * sizeof(double));

  omp_set_num_threads(14);
  for (int k = 0; k != step; ++k) {
#pragma omp parallel
    {
      int threads = omp_get_num_threads();
      int threads_id = omp_get_thread_num();
      int chunk_size = N / threads;
      int start = threads_id * chunk_size + 1;
      int end = (threads_id == threads - 1) ? N - 1 : start + chunk_size;
      __m256d set = _mm256_set1_pd(0.25);
      for (int i = start; i != end; ++i) {
        int j;
        for (j = 1; j < N - 16; j += 16) {
          __m256d top1 = _mm256_loadu_pd(&p[(i - 1) * N + j]);
          __m256d bottom1 = _mm256_loadu_pd(&p[(i + 1) * N + j]);
          __m256d left1 = _mm256_loadu_pd(&p[i * N + j - 1]);
          __m256d right1 = _mm256_loadu_pd(&p[i * N + j + 1]);
          __m256d avg1 = _mm256_add_pd(_mm256_add_pd(top1, bottom1),
                                       _mm256_add_pd(left1, right1));
          avg1 = _mm256_mul_pd(avg1, set);
          _mm256_storeu_pd(&p_next[i * N + j], avg1);

          __m256d top2 = _mm256_loadu_pd(&p[(i - 1) * N + j + 4]);
          __m256d bottom2 = _mm256_loadu_pd(&p[(i + 1) * N + j + 4]);
          __m256d left2 = _mm256_loadu_pd(&p[i * N + j + 3]);
          __m256d right2 = _mm256_loadu_pd(&p[i * N + j + 5]);
          __m256d avg2 = _mm256_add_pd(_mm256_add_pd(top2, bottom2),
                                       _mm256_add_pd(left2, right2));
          avg2 = _mm256_mul_pd(avg2, set);
          _mm256_storeu_pd(&p_next[i * N + j + 4], avg2);

          __m256d top3 = _mm256_loadu_pd(&p[(i - 1) * N + j + 8]);
          __m256d bottom3 = _mm256_loadu_pd(&p[(i + 1) * N + j + 8]);
          __m256d left3 = _mm256_loadu_pd(&p[i * N + j + 7]);
          __m256d right3 = _mm256_loadu_pd(&p[i * N + j + 9]);
          __m256d avg3 = _mm256_add_pd(_mm256_add_pd(top3, bottom3),
                                       _mm256_add_pd(left3, right3));
          avg3 = _mm256_mul_pd(avg3, set);
          _mm256_storeu_pd(&p_next[i * N + j + 8], avg3);

          __m256d top4 = _mm256_loadu_pd(&p[(i - 1) * N + j + 12]);
          __m256d bottom4 = _mm256_loadu_pd(&p[(i + 1) * N + j + 12]);
          __m256d left4 = _mm256_loadu_pd(&p[i * N + j + 11]);
          __m256d right4 = _mm256_loadu_pd(&p[i * N + j + 13]);
          __m256d avg4 = _mm256_add_pd(_mm256_add_pd(top4, bottom4),
                                       _mm256_add_pd(left4, right4));
          avg4 = _mm256_mul_pd(avg4, set);
          _mm256_storeu_pd(&p_next[i * N + j + 12], avg4);
        }
        for (; j != N - 1; ++j) {
          p_next[i * N + j] = (p[(i - 1) * N + j] + p[(i + 1) * N + j] +
                               p[i * N + j + 1] + p[i * N + j - 1]) /
                              4.0f;
        }
      }
    }
    double *temp = p;
    p = p_next;
    p_next = temp;
  }
  memcpy(p_p, p, N * N * sizeof(double));
  free(p_p_next);
  free(p_copy);
}

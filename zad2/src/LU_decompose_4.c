#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <float.h>
#include <x86intrin.h>

#define IDX(i, j, n) (((j)+ (i)*(n)))
static double gtod_ref_time_sec = 0.0;

int LUPDecompose(double *A, int N) {
    register int i, j, k, size;
    register double division_var;
    register __m128d tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, mult;
    size = N;

    for (i = 0; i < size; i++) {
        division_var = A[IDX(i, i, size)];
        for (j = i + 1; j < size; j++) {
            A[IDX(j, i, size)] /= division_var;
            mult[0] = A[IDX(j, i, size)];
            mult[1] = A[IDX(j, i, size)];
            for (k = i + 1; k < size;) {
                if (k + 7 < size) {
                    tmp0 = _mm_loadu_pd(A + IDX(i, k, size));
                    tmp1 = _mm_loadu_pd(A + IDX(i, k + 2, size));
                    tmp2 = _mm_loadu_pd(A + IDX(i, k + 4, size));
                    tmp3 = _mm_loadu_pd(A + IDX(i, k + 6, size));

                    tmp4 = _mm_loadu_pd(A + IDX(j, k, size));
                    tmp5 = _mm_loadu_pd(A + IDX(j, k + 2, size));
                    tmp6 = _mm_loadu_pd(A + IDX(j, k + 4, size));
                    tmp7 = _mm_loadu_pd(A + IDX(j, k + 6, size));

                    tmp0 = _mm_mul_pd(mult, tmp0);
                    tmp1 = _mm_mul_pd(mult, tmp1);
                    tmp2 = _mm_mul_pd(mult, tmp2);
                    tmp3 = _mm_mul_pd(mult, tmp3);

                    tmp0 = _mm_sub_pd(tmp4, tmp0);
                    tmp1 = _mm_sub_pd(tmp5, tmp1);
                    tmp2 = _mm_sub_pd(tmp6, tmp2);
                    tmp3 = _mm_sub_pd(tmp7, tmp3);

                    A[IDX(j, k, size)] = tmp0[0];
                    A[IDX(j, k + 1, size)] = tmp0[1];
                    A[IDX(j, k + 2, size)] = tmp1[0];
                    A[IDX(j, k + 3, size)] = tmp1[1];
                    A[IDX(j, k + 4, size)] = tmp2[0];
                    A[IDX(j, k + 5, size)] = tmp2[1];
                    A[IDX(j, k + 6, size)] = tmp3[0];
                    A[IDX(j, k + 7, size)] = tmp3[1];
                    k += 8;
                } else {
                    A[IDX(j, k, size)] -= A[IDX(j, i, size)] * A[IDX(i, k, size)];
                    k++;
                }
            }
        }
    }

    return 0;
}

double dclock() {
    double the_time, norm_sec;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (gtod_ref_time_sec == 0.0)
        gtod_ref_time_sec = (double) tv.tv_sec;
    norm_sec = (double) tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + tv.tv_usec * 1.0e-6;
    return the_time;
}

double calculate_gflops(int size) {
    double flops = 0;
    for (int i = 0; i < size; i++) {
        flops += (size - i - 1) + 2 * (size - i - 1) * (size - i - 1);
    }
    return flops * 1.0e-09;
}

int main() {
    int i, j, reps = 5;
    double dtime, dtime_best = FLT_MAX;
    double *matrix;

    for (int size = 40; size <= 1000; size += 40) {
        matrix = malloc(size * size * sizeof(double));
        srand(1);

        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                matrix[IDX(i, j, size)] = rand();
            }
        }

        for (int rep = 0; rep < reps; rep++) {
            dtime = dclock();
            LUPDecompose(matrix, size);
            dtime = dclock() - dtime;
            if (rep == 0) {
                dtime_best = dtime;
            } else {
                dtime_best = (dtime < dtime_best ? dtime : dtime_best);
            }
        }

        double check = 0.0;
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                check = check + matrix[IDX(i, j, size)];
            }
        }
        printf("%d %le %le\n", size, calculate_gflops(size) / dtime_best, check);
        fflush(stdout);
        free(matrix);
    }
}

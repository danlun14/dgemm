#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>
#include "hpctimer.h"

enum
{
    NREPS = 3
};

char *plot_file[] = {
    "dgemm.txt",
    "dgemm_t.txt"
    "dgemm_b.txt",
    "dgemm_avx.txt",
    "dgemm_bs.txt"};

void dgemm_def(float *a, float *b, float *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}

void dgemm_transpose(const float *a, const float *b, float *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (k = 0; k < n; k++)
        {
            for (j = 0; j < n; j++)
            {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}
/*
void dgemm_avx(const float *A, const float *B, float *C, int n)
{
    for (int i = 0; i < n; ++i)
    {
        float *c = C + i * n;
        for (int k = 0; k < n; ++k)
        {
            const float *b = B + k * n;
            __m256 a = _mm256_set1_ps(A[i * n + k]);
            for (int j = 0; j < n; j += 16)
            {
                __m256 fmad1 = _mm256_fmadd_ps(a, _mm256_loadu_ps(b + j + 0), _mm256_loadu_ps(c + j + 0));
                _mm256_storeu_ps(c + j + 0, fmad1);
                _mm256_storeu_ps(c + j + 8, _mm256_fmadd_ps(a, _mm256_loadu_ps(b + j + 8), _mm256_loadu_ps(c + j + 8)));
            }
        }
    }
}
*/
void dgemm_block(float *a, float *b, float *c, int n, int BS)
{
    int i, j, k;
    int i0, j0, k0;
    float *a0, *b0, *c0;

    for (i = 0; i < n; i += BS)
    {
        for (j = 0; j < n; j += BS)
        {
            for (k = 0; k < n; k += BS)
            {
                for (i0 = 0, c0 = (c + i * n + j),
                    a0 = (a + i * n + k);
                     i0 < BS;
                     ++i0, c0 += n, a0 += n)
                {
                    for (k0 = 0, b0 = (b + k * n + j);
                         k0 < BS; ++k0, b0 += n)
                    {
                        for (j0 = 0; j0 < BS; ++j0)
                        {
                            c0[j0] += a0[k0] * b0[j0];
                        }
                    }
                }
            }
        }
    }
}

void init_matrix(float *a, float *b, float *c, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            for (k = 0; k < n; k++)
            {
                *(a + i * n + j) = 1.0f;
                *(b + i * n + j) = 2.0f;
                *(c + i * n + j) = 0.0f;
            }
        }
    }
}

void print_matrix(float *a, int n)
{
    int i, j;

    printf("Matrix:\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%12.2f", *(a + i * n + j));
        }
        printf("\n");
    }
}

void cpuid(int fn, unsigned int *eax, unsigned int *ebx,
           unsigned int *ecx, unsigned int *edx)
{
    asm volatile("cpuid"
                 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                 : "a"(fn));
}
int is_avx_supported()
{
    unsigned int eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);
    return (ecx & (1 << 28)) ? 1 : 0;
}

int main(int argc, char **argv)
{
//    int N = 1024;
    //int N = 2048;
    int N = 512;
    int BS;
    float *A = (float *)_mm_malloc(sizeof(float) * N * N, 16);
    float *B = (float *)_mm_malloc(sizeof(float) * N * N, 16);
    float *C = (float *)_mm_malloc(sizeof(float) * N * N, 16);
    int index = 0;
    init_matrix(A, B, C, N);
    printf("initializated\n");
    int i;
    double t, t_start;
//    for (BS = 2; BS < 512; BS *= 2)
//    {
        t = hpctimer_getwtime();
        for (i = 0; i < NREPS; i++)
        {
//            dgemm_def(A, B, C, N);
            dgemm_transpose(A, B, C, N);
            //dgemm_avx(A, B, C, N);
//            dgemm_block(A, B, C, N, BS);
//            printf("%d -> success!\n", i);
        }
        t = hpctimer_getwtime() - t;
        t = t / NREPS;
//        printf("Elapsed time: %d %.6f sec.\n\n", BS, t);
    //}
    free(A);
    free(B);
    free(C);
    return 0;
}
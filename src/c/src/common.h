#include <stdio.h>

#define pi 3.14159

typedef struct complex {
    double real;
    double imag;
} complex;

struct fft_info {
    int len;
    complex *input;
    complex *output;
};

// numpy.arange(strat, stop, step, dtype=None)
double *arange(double start, double stop, double step) 
{
    int N = (int)((1 / step) * stop);
    double *array = calloc(N, sizeof(double));

    for (int index = 0; index < N; index++) {
        array[index] = start + index * step;
        //printf("%d %f\n", index, array[index]);
    }
    
    return array;
}

void print_array(double *x, int N)
{
    printf("[");
    for (int i = 0; i < N; i++) {
        if (i % 8 == 0 && i != 0) {
            printf("\n ");
        }
        printf("%6.2f%s", x[i], (i == N-1 ? "\0" : ","));
    }
    printf("]\n");
}

void print_complex_array(complex *x, int N)
{
    printf("[");
    for (int i = 0; i < N; i++) {
        if (i % 4 == 0 && i != 0) {
            printf("\n ");
        }
        printf("%+6.2f%+.2f%s", x[i].real, x[i].imag, (i == N-1 ? "j" : "j, "));
    }
    printf("]\n");
}

void print_binary(unsigned int n) {
    if (n == 0) {
        printf("0");
        return;
    }

    unsigned int x;
    for (int bit = 31; bit >= 0; bit--) {
        x = n >> bit;

        if (x & 1) printf("1");
        else printf("0");
    }
    printf("\n");
}

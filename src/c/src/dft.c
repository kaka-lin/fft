#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "common.h"

complex *DFT(double *x, int N)
{   
    clock_t start, end;
    start = clock();
    
    complex *y = calloc(N, sizeof(complex));

    for (int k = 0; k < N; k++) {
        double real = 0.0;
        double imag = 0.0;
        
        for (int n = 0; n < N; n++) {
            real += x[n] * cos(2 * pi *k * n / N);
            imag += ((-1) * x[n] * sin(2 * pi * k * n / N));
        }

        y[k].real = real;
        y[k].imag = imag;
    }

    end = clock();
    printf("DFT, N: %d, spends time: %lfs\n", N, (end-start) / (double)(CLOCKS_PER_SEC));

    return y;
}

complex *IDFT(complex *x, int N)
{ 
    complex *y = calloc(N, sizeof(complex));

    for (int n = 0; n < N; n++) {
        double real = 0.0;
        double imag = 0.0;

        for (int k = 0; k < N; k++) {
            real += x[k].real * cos(2 * pi * k * n / N) - x[k].imag * sin(2 * pi * k * n / N);
            imag += x[k].real * sin(2 * pi * k * n / N) + x[k].imag * cos(2 * pi * k * n / N);
        }

        y[n].real = real / N;
        y[n].imag = imag / N;
    }

    return y;
}

int main(int argc, char **argv)
{   
    /* Sampling information */
    int fs = 8;
    double Ts = 1.0 / fs;
    double t = 1.0;
    int N = fs * t;
    
    /* signal information */ 
    int f = 1;
    double *x = arange(0, t, Ts);
    
    // y = cos(2*pi*f*x)
    double y[N]; 
    for (int i = 0; i < N; i++) {
        y[i] = cos(2 * pi * f * x[i]);
    }

    printf("Input:\n");
    print_array(y, N);

    // Discrete Fourier Tranform (DFT)
    // According to the DFT formula, 
    // time complexity: O(n^2)
    complex *yf = DFT(y, N);
    printf("DFT:\n");
    print_complex_array(yf, N);

    complex *yif = IDFT(yf, N);
    //printf("IDFT:\n");
    //print_complex_array(yif, N);
   
    return 0;
}


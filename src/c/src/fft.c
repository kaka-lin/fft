#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "common.h"

int is_two_n(int num)
{
    /* ex: num = 8, 8-1 = 7
     *   
     * 8:　    1000
     * 7:　　& 0111
     *      -------
     *        0000
     */ 
	if ((num & (num - 1)) == 0)
		return 1;
	return 0;
}

// bit-reversal permutation
void bit_reverse(struct fft_info *fft_info)
{
    int N = fft_info->len;
    int bits;

    if (!is_two_n(N)) {
        bits = log2(N) + 1;
        N = pow(2, bits);
    } else {
        bits = log2(N);
    }

    complex *reverse = calloc(N, sizeof(complex));
    int reverse_index;

    for (int i = 0; i < N; i++) {
        reverse_index = 0;
        for (int j = 0; j < bits; j++) { 
            if ((i >> j) & 1) {
                reverse_index |= (1 << (bits - j - 1));
            }
        }

        if (reverse_index >= fft_info->len)
            reverse[i].real = 0.0;
        else
            reverse[i].real = fft_info->input[reverse_index].real;
    }

    fft_info->input = reverse;
    fft_info->len = N;
}

/* Cooley-Tukey Fast Fourier Transform(FFT) Algorithm
 *
 * 1. radix-2 decimation-in-time (DIT) FFT
 *    -> O(NlogN)
 */
void FFT(struct fft_info *fft_info)
{   
    clock_t start, end;
    start = clock();

    // bit-reversal permutation */
    bit_reverse(fft_info);

    int N = fft_info->len;
    complex *x = fft_info->input;

    complex W, a, b;

    /* dynamic programming */ 
    //1. stage (N = 8), 取log => 3
    //   3個stage: 1 -> 2 -> 3
    //    ->　DFT架構(Butterfly跨越的數): 2 -> 4 -> 8  
    for (int k = 2; k <= N; k<<=1) {
        //printf("Stage %.f: \n", log2((double)k));

        // 2. 每個stage做一次FFT
        //   每個stage分成 (N / 2^s(=k)) 個 group
        for (int g = 0; g < N; g += k) {
            // 3. 因為對稱所以一起做
            //   stage1:   stage2:            stage3:
            //    0, 1    0, 2 & 1, 3 
            //    2, 3                  0,4 & 1,5 & 2,6 & 3,7
            //    4, 5    4, 6 & 5, 7              
            //    6, 7  
            memset(&W, 0, sizeof(W));

            //printf("\t");
            for (int i = g; i < g + k/2; i++) {
                // 公式: 
                //     W^(k/N), k = 每個group的DFT架構的even
                // stage1: N = 2 (N/4)
                // stage2: N = 4 (N/2)
                // stage3: N = 8 (N)
                W.real = cos(2 * pi * i / k); 
                W.imag = (-1) * sin(2 * pi * i / k);
                //printf("%d %d, w: %.2f%+.2fj, ", i, i + k / 2, W.real, W.imag);
                
                // 公式: 
                //     X[k] = x[2r],even + W * x[2r+1],odd
                // 
                // W * x[n+1] = (a+jb)*(c+jd)
                //            = (ac-bd)+j(ad+bc)
                a.real = x[i].real;
                a.imag = x[i].imag;
                b.real = W.real * x[i + k / 2].real - W.imag * x[i + k / 2].imag;
                b.imag = W.real * x[i + k / 2].imag + W.imag * x[i + k / 2].real;

                // 公式:
                //     X[k]     = even + W * odd
                //     X[k+N/2] = even - W * odd
                x[i].real = a.real + b.real;
                x[i].imag = a.imag + b.imag;
                x[i + k / 2].real = a.real - b.real;
                x[i + k / 2].imag = a.imag - b.imag;
            }
            //printf("\n");
        }
        //printf("End, next stage!\n");
    }

    end = clock();
    printf("FFT, N: %d, spends time: %lfs\n", N, (end-start) / (double)(CLOCKS_PER_SEC));

    fft_info->output = x;
}

int main(int argc, char **argv)
{       
    struct fft_info fft_info;

    /* Sampling information */
    int fs = 8;
    double Ts = 1.0 / fs;
    double t = 1.0;
    int N = fs * t;
    
    /* signal information */
    int f = 1;
    double *x = arange(0, t, Ts);
    //printf("time:\n");
    //print_array(x, N);

    // y = cos(2*pi*f*x)
    complex *y = calloc(N, sizeof(complex)); 
    for (int i = 0; i < N; i++) {
        y[i].real = cos(2 * pi * f * x[i]);
    }

    fft_info.len = N;
    fft_info.input = y;
    printf("Input array:\n");
    print_complex_array(fft_info.input, fft_info.len);

    /* Fast Fourier Tranform (FFT) */
    FFT(&fft_info);
    printf("FFT:\n");
    print_complex_array(fft_info.output, fft_info.len);

    return 0;
}

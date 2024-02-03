/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    ComplexNumber * z = newComplexNumber(0, 0);
    for (u_int64_t i = 0; i < maxiters; ++i) {
        z = ComplexSum(ComplexProduct(z, z), point);
        double z_len = ComplexAbs(z);
        if (z_len >= threshold) {
            return i + 1;
        }
    }
    return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    double step = scale / resolution;
    u_int64_t num = 0;
    for (int i = 0; i < 2 * resolution + 1; ++i) {
        for (int j = 0; j < 2 * resolution + 1; ++j) {
            double real = Re(center) + (j - (int)resolution) * step;
            double imaginary = Im(center) + ((int)resolution - i) * step;
            ComplexNumber * point = newComplexNumber(real, imaginary);
            u_int64_t iterations = MandelbrotIterations(max_iterations, point, threshold);
            output[num++] = iterations;
            freeComplexNumber(point);
        }
    }
}



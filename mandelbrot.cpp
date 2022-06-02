#include <complex>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>

using namespace cv;
using namespace std;

int main(void) {
    // number of grid points in real and imag component
    int n = 4096;

    // area to calculate
    long double min_re, max_re, min_im, max_im;
    min_re = -2.;
    max_re = 2.;
    min_im = -2.;
    max_im = 2.;

    // delta in real and imaginary part
    long double delta_re, delta_im;
    delta_re = max_re - min_re;
    delta_im = max_im - min_im;

    // image
    Mat img(n, n, CV_8UC1);

    // calculate the pixels of the image
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            complex<long double> c = complex<long double>(
                min_re + delta_re * (long double)(i) / (long double)(n),
                min_im + delta_im * (long double)(j) / (long double)(n));

            if (norm(c) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            complex<long double> z = c;

            uint8_t k_write = 255;
            for (uint8_t k = 1; k < 255; k++) {
                z = z * z + c;

                if (norm(z) > 2) {
                    k_write = k;
                    break;
                }
            }

            img.at<uint8_t>(j, i) = k_write;
        }
    }

    // save the image
    imwrite("mandelbrot.png", img);
}
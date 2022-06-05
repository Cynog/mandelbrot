#include <omp.h>

#include <complex>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int intpow(int base, int exp);

int main(void) {
    // number of grid points in real and imag component for each tile
    int n = 1024;

    // point to zoom in
    long double re = -4.621603e-1;
    long double im = -5.823998e-1;

    for (int z = 0; z <= 16; z++) {  // zoom
        // delta in real and imaginary part
        long double delta_re, delta_im;
        delta_re = 4. / (long double)(intpow(2, z));
        delta_im = 4. / (long double)(intpow(2, z));

        // area to calculate
        long double min_re, min_im;
        min_re = re - delta_re / 2.0;
        min_im = im - delta_im / 2.0;

        // print information on the current rendering region
        printf("z=%d\n", z);

        // empty image
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
        char filename[100];
        sprintf(filename, "zoom/z%d.png", z);
        imwrite(filename, img);
    }
}

int intpow(int base, int exp) {
    int result = 1;

    for (int k = 0; k < exp; k++) {
        result *= base;
    }

    return result;
}
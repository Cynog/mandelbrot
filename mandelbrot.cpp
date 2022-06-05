#include "mandelbrot.hpp"

int intpow(int base, int exp) {
    int result = 1;

    for (int k = 0; k < exp; k++) {
        result *= base;
    }

    return result;
}

cv::Vec3b colormap(int k) {
    // RGB triple to return
    cv::Vec3b pixel;

    // return the RGB triple
    return pixel;
}
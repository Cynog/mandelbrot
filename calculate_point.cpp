#include <omp.h>

#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "mandelbrot.hpp"

using namespace cv;
using namespace std;

int main(void) {
    // number of grid points in real and imag component for each tile
    int res = 1024;

    // point to zoom in
    long double re = -0.462160252968;
    long double im = -0.582399836787;

    // calculate the corresponding point from the image
    complex<long double> c = calculate_point_from_zoom_image(32, re, im, 604, 167, res, res);

    // print the point
    printf("The point is %.30Lf %.30Lf\n", c.real(), c.imag());
}
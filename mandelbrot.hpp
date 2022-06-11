#pragma once

#include <gmp.h>
#include <mpfr.h>
#include <omp.h>

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define THREADNUM omp_get_max_threads()
#define PREC 80
#define RND MPFR_RNDD

unsigned long int intpow(unsigned int base, int exp);

cv::Vec3b colormap(int k);

cv::Mat render_image(mpfr_t &x_min, mpfr_t &y_min, mpfr_t &delta_x, mpfr_t &delta_y, int resx, int resy);

long double mpfr_norm(mpfr_t &x, mpfr_t &y);

//TODO std::complex<mpfr_t> calculate_point_from_zoom_image(int z, mpfr_t re, mpfr_t im, int pixelx, int pixely, int resx, int resy);
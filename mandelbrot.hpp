#pragma once

#include <gmp.h>
#include <omp.h>

#include <complex>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

long long int intpow(int base, int exp);

cv::Vec3b colormap(int k);

cv::Mat render_image(long double re_min, long double im_min,
                     long double delta_re, long double delta_im, int resx,
                     int resy);

std::complex<long double> calculate_point_from_zoom_image(int z, long double re, long double im, int pixelx, int pixely, int resx, int resy);
#pragma once

#include <omp.h>

#include <complex>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int intpow(int base, int exp);

cv::Vec3b colormap(int k);

cv::Mat render_image(long double re_min, long double im_min,
                     long double delta_re, long double delta_im, int resx,
                     int resy);
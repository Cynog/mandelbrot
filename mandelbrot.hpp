#pragma once

#include <gmp.h>
#include <mpfr.h>
#include <omp.h>

#include <complex>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#define THREADNUM omp_get_max_threads()
#define PREC 80
#define RND MPFR_RNDD

cv::Vec3b colormap(int k);

cv::Mat render_image_f(float x_min, float y_min, float delta_x, float delta_y, int res_x, int res_y);
cv::Mat render_image_d(double x_min, double y_min, double delta_x, double delta_y, int res_x, int res_y);
cv::Mat render_image_ld(long double x_min, long double y_min, long double delta_x, long double delta_y, int res_x, int res_y);
cv::Mat render_image_mpfr(mpfr_t &x_min, mpfr_t &y_min, mpfr_t &delta_x, mpfr_t &delta_y, int resx, int resy);

long double mpfr_norm(mpfr_t &x, mpfr_t &y);

//TODO std::complex<mpfr_t> calculate_point_from_zoom_image(int z, mpfr_t re, mpfr_t im, int pixelx, int pixely, int resx, int resy);
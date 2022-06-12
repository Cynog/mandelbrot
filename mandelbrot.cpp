#include "mandelbrot.hpp"

cv::Vec3b colormap(int k) {  // TODO
    // RGB triple to return
    cv::Vec3b pixel;

    // return the RGB triple
    return pixel;
}

cv::Mat render_image_f(float x_min, float y_min, float delta_x, float delta_y, int res_x, int res_y) {
    // empty image grayscale and color
    cv::Mat img(res_y, res_x, CV_8UC1);
    cv::Mat img_color;

    // calculate the pixels of the image
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (int i = 0; i < res_x; i++) {
        for (int j = 0; j < res_y; j++) {
            std::complex<float> c = std::complex<float>(x_min + delta_x * (float)(i) / (float)(res_x), y_min + delta_y * (float)(j) / (float)(res_y));

            if (norm(c) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            std::complex<float> z = c;

            uint8_t k_write = 255;
            for (int k = 1; k < 255*100; k++) {
                z = z * z + c;

                if (norm(z) > 2) {
                    k_write = k%255;
                    break;
                }
            }

            img.at<uint8_t>(j, i) = k_write;
        }
    }
    // apply colormap
    applyColorMap(img, img_color, cv::COLORMAP_JET);

    // return image
    return img_color;
}

cv::Mat render_image_d(double x_min, double y_min, double delta_x, double delta_y, int res_x, int res_y) {
    // empty image grayscale and color
    cv::Mat img(res_y, res_x, CV_8UC1);
    cv::Mat img_color;

    // calculate the pixels of the image
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (int i = 0; i < res_x; i++) {
        for (int j = 0; j < res_y; j++) {
            std::complex<double> c = std::complex<double>(x_min + delta_x * (double)(i) / (double)(res_x), y_min + delta_y * (double)(j) / (double)(res_y));

            if (norm(c) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            std::complex<double> z = c;

            uint8_t k_write = 255;
            for (int k = 1; k < 255*100; k++) {
                z = z * z + c;

                if (norm(z) > 2) {
                    k_write = k%255;
                    break;
                }
            }

            img.at<uint8_t>(j, i) = k_write;
        }
    }
    // apply colormap
    applyColorMap(img, img_color, cv::COLORMAP_JET);

    // return image
    return img_color;
}

cv::Mat render_image_ld(long double x_min, long double y_min, long double delta_x, long double delta_y, int res_x, int res_y) {
    // empty image grayscale and color
    cv::Mat img(res_y, res_x, CV_8UC1);
    cv::Mat img_color;
    
    // calculate the pixels of the image
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (int i = 0; i < res_x; i++) {
        for (int j = 0; j < res_y; j++) {
            std::complex<long double> c = std::complex<long double>(x_min + delta_x * (long double)(i) / (long double)(res_x), y_min + delta_y * (long double)(j) / (long double)(res_y));

            if (norm(c) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            std::complex<long double> z = c;

            uint8_t k_write = 255;
            for (int k = 1; k < 255*100; k++) {
                z = z * z + c;

                if (norm(z) > 2) {
                    k_write = k%255;
                    break;
                }
            }

            img.at<uint8_t>(j, i) = k_write;
        }
    }
    // apply colormap
    applyColorMap(img, img_color, cv::COLORMAP_JET);

    // return image
    return img_color;
}

cv::Mat render_image_mpfr(mpfr_t &x_min, mpfr_t &y_min, mpfr_t &delta_x, mpfr_t &delta_y, int res_x, int res_y) {
    // empty image grayscale and color
    cv::Mat img(res_y, res_x, CV_8UC1);
    cv::Mat img_color;

// calculate the pixels of the image
#pragma omp parallel for num_threads(THREADNUM)
    for (int i = 0; i < res_x; i++) {
        for (int j = 0; j < res_y; j++) {
            mpfr_t c_x; // = x_min + delta_x * i/res_x
            mpfr_init2(c_x, PREC);
            mpfr_set_si(c_x, i, RND);
            mpfr_div_si(c_x, c_x, res_x, RND);
            mpfr_mul(c_x, delta_x, c_x, RND);
            mpfr_add(c_x, x_min, c_x, RND);
            mpfr_t c_y; // = y_min + delta_y * j/res_y
            mpfr_init2(c_y, PREC);
            mpfr_set_si(c_y, j, RND);
            mpfr_div_si(c_y, c_y, res_y, RND);
            mpfr_mul(c_y, delta_y, c_y, RND);
            mpfr_add(c_y, y_min, c_y, RND);

            // check divergence
            if (mpfr_norm(c_x, c_y) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            // z = c
            mpfr_t z_x, z_y;
            mpfr_init2(z_x, PREC);
            mpfr_init2(z_y, PREC);
            mpfr_set(z_x, c_x, RND);
            mpfr_set(z_y, c_y, RND);

            // additional mpfr variables for calculation
            mpfr_t z_x_2;
            mpfr_t z_y_2;
            mpfr_init2(z_x_2, PREC);
            mpfr_init2(z_y_2, PREC);

            // iterate
            uint8_t k_write = 255;
            for (int k = 1; k < 255*40; k++) {
                // z = z*z + c
                // square z_x and z_y
                mpfr_pow_si(z_x_2, z_x, 2, RND);
                mpfr_pow_si(z_y_2, z_y, 2, RND);

                // calculate z*z
                mpfr_mul(z_y, z_x, z_y, RND);
                mpfr_mul_si(z_y, z_y, 2, RND);
                mpfr_sub(z_x, z_x_2, z_y_2, RND);

                // add c
                mpfr_add(z_x, z_x, c_x, RND);
                mpfr_add(z_y, z_y, c_y, RND);

                // check divergence
                if (mpfr_norm(z_x, z_y) > 2) {
                    k_write = k % 255;
                    break;
                }
            }

            // free additional mpfr variables
            mpfr_clear(z_x_2);
            mpfr_clear(z_y_2);

            // set the images pixel value
            img.at<uint8_t>(j, i) = k_write;
        }
    }
    // apply colormap
    applyColorMap(img, img_color, cv::COLORMAP_JET);

    // return image
    return img_color;
}

long double mpfr_norm(mpfr_t &x, mpfr_t &y) {
    // additional mpfr variables for calculation
    mpfr_t x_2;
    mpfr_t y_2;
    mpfr_init2(x_2, PREC);
    mpfr_init2(y_2, PREC);

    // square x and y
    mpfr_pow_si(x_2, x, 2, RND);
    mpfr_pow_si(y_2, y, 2, RND);

    // sum and take sqrt
    mpfr_add(x_2, x_2, y_2, RND);
    mpfr_sqrt(x_2, x_2, RND);

    // convert to long double
    long double norm = mpfr_get_ld(x_2, RND);

    // free additional mpfr variables
    mpfr_clear(x_2);
    mpfr_clear(y_2);

    // return
    return norm;
}

//TODO
/** std::complex<mpfr_t> calculate_point_from_zoom_image(int z, mpfr_t re,
                                                     mpfr_t im, int i, int j,
                                                     int res_x, int res_y) {
    // delta in real and imaginary part
    mpfr_t delta_x = 4. / (mpfr_t)(intpow(2, z));
    mpfr_t delta_y = 4. / (mpfr_t)(intpow(2, z));

    // area to calculate
    mpfr_t x_min = re - delta_x / 2.0;
    mpfr_t y_min = im - delta_y / 2.0;

    // calculate the corresponding point
    std::complex<mpfr_t> c =
        std::complex<mpfr_t>(x_min + delta_x * (mpfr_t)(i) / (mpfr_t)(res_x),
                             y_min + delta_y * (mpfr_t)(j) / (mpfr_t)(res_y));

    // return the corresponding point
    return c;
}**/
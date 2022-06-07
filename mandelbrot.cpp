#include "mandelbrot.hpp"

long int intpow(int base, int exp) {
    long int result = 1;

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

cv::Mat render_image(long double re_min, long double im_min, long double delta_re, long double delta_im, int resx, int resy) {
    // empty image grayscale and color
    cv::Mat img(resy, resx, CV_8UC1);
    cv::Mat img_color;

    // calculate the pixels of the image
    #pragma omp parallel for num_threads(omp_get_max_threads())
    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            std::complex<long double> c = std::complex<long double>(re_min + delta_re * (long double)(i) / (long double)(resx), im_min + delta_im * (long double)(j) / (long double)(resy));

            if (norm(c) > 2) {
                img.at<uint8_t>(j, i) = 0;
                continue;
            }

            std::complex<long double> z = c;

            uint8_t k_write = 255;
            for (int k = 1; k < 255*50; k++) {
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

std::complex<long double> calculate_point_from_zoom_image(int z, long double re, long double im, int i, int j, int resx, int resy) {
    // delta in real and imaginary part
    long double delta_re = 4. / (long double)(intpow(2, z));
    long double delta_im = 4. / (long double)(intpow(2, z));

    // area to calculate
    long double re_min = re - delta_re / 2.0;
    long double im_min = im - delta_im / 2.0;

    // calculate the corresponding point
    std::complex<long double> c = std::complex<long double>(re_min + delta_re * (long double)(i) / (long double)(resx), im_min + delta_im * (long double)(j) / (long double)(resy));

    // return the corresponding point
    return c;
}
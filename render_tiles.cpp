#include <omp.h>

#include <complex>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "mandelbrot.hpp"

namespace fs = std::filesystem;

int main(void) {
    // number of grid points in real and imag component for each tile
    int res = 1024;

    // create folder
    fs::create_directory("tiles/");
    for (int z = 0; z <= 2; z++) {  // zoom
        // create subfolder
        char dir[100];
        sprintf(dir, "tiles/z%d/", z);
        fs::create_directory(dir);

        for (int it = 0; it < intpow(2, z); it++) {      // tiles rows
            for (int jt = 0; jt < intpow(2, z); jt++) {  // tiles colums
                // delta in real and imaginary part
                long double delta_re = 4. / (long double)(intpow(2, z));
                long double delta_im = 4. / (long double)(intpow(2, z));

                // area to calculate
                long double re_min = -2. + delta_re * it;
                long double im_min = -2. + delta_im * jt;

                // print information on the current rendering region
                printf("z=%d   it=%d   jt=%d   min_re=%Lg   min_im=%Lg\n", z, it, jt, re_min, im_min);

                // render the image
                cv::Mat img = render_image(re_min, im_min, delta_re, delta_im, res, res);

                // save the image
                char filename[100];
                sprintf(filename, "%s%d_%d.png", dir, it, jt);
                imwrite(filename, img);
            }
        }
    }
}
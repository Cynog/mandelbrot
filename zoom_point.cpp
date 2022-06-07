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
    long double re = -0.462160252884326494537958751607;
    long double im = -0.582399837100775696896448607731;

    for (int z = 0; z <= 37; z++) {  // zoom
        // delta in real and imaginary part
        long double delta_re, delta_im;
        delta_re = 4. / (long double)(intpow(2, z));
        delta_im = 4. / (long double)(intpow(2, z));

        // area to calculate
        long double re_min = re - delta_re / 2.0;
        long double im_min = im - delta_im / 2.0;

        // print information on the current rendering region
        printf("z=%d\n", z);

        // render the image
        Mat img = render_image(re_min, im_min, delta_re, delta_im, res, res);

        // save the image
        char filename[100];
        sprintf(filename, "zoom/z%d.png", z);
        imwrite(filename, img);
    }
}
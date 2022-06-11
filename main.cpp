#include <gmp.h>
#include <mpfr.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "mandelbrot.hpp"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    // prepare parsing command line arguments
    char ch;
    int p = 0;  // standart program is zoom
    int z_min = 0, z_max = 4;
    mpfr_t x, y;
    mpfr_init2(x, PREC);
    mpfr_init2(y, PREC);
    int res = 512;
    int pixel_x = 0, pixel_y = 0;
    char *val;

    // parse command line arguments
    while ((ch = getopt(argc, argv, "p:z:x:y:r:i:")) != EOF) {
        switch (ch) {
            case 'p':  // program zoom or program tiles
                if (strcmp(optarg, "zoom") == 0) {
                    p = 0;
                } else if (strcmp(optarg, "tiles") == 0) {
                    p = 1;
                } else if (strcmp(optarg, "calcpointfromimage") == 0) {
                    p = 2;
                } else {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                break;
            case 'z':  // zoom values
                // TODO allow things like -z :8 for p1 or -z 32 for p3
                val = strtok(optarg, ":");
                if (val == NULL) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                z_min = strtol(val, NULL, 10);

                val = strtok(NULL, ":");
                if (val == NULL) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                z_max = strtol(val, NULL, 10);
                break;
            case 'x':
                mpfr_set_str(x, optarg, 10, RND);
                break;
            case 'y':
                mpfr_set_str(y, optarg, 10, RND);
                break;
            case 'r':
                res = strtol(optarg, NULL, 10);
                break;
            case 'i':
                val = strtok(optarg, ":");
                if (val == NULL) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                pixel_x = strtol(val, NULL, 10);

                val = strtok(NULL, ":");
                if (val == NULL) {
                    std::cout << "error" << std::endl;
                    return 1;
                }
                pixel_y = strtol(val, NULL, 10);
                break;
            default:
                return 1;
        }
    }

    // print all the parameters after parsing
    std::cout << "p " << p << std::endl;
    std::cout << "x " << mpfr_get_ld(x, RND) << std::endl;
    std::cout << "y " << mpfr_get_ld(y, RND) << std::endl;
    std::cout << "z_min " << z_min << std::endl;
    std::cout << "z_max " << z_max << std::endl;
    std::cout << "res " << res << std::endl;
    std::cout << "pixel_x " << pixel_x << std::endl;
    std::cout << "pixel_y " << pixel_y << std::endl;
    std::cout << std::endl;

    // execute the requested programs
    if (p == 0) {
        // create folder
        char dir[50] = "zoom/";
        fs::create_directory(dir);

        // calculate the zoomed images
        for (int z = z_min; z <= z_max; z++) {  // zoom
            // print information on the current rendering region
            printf("z=%d\n", z);

            // delta in real and imaginary part
            mpfr_t delta_x, delta_y;
            mpfr_init2(delta_x, PREC);
            mpfr_init2(delta_y, PREC);
            mpfr_set_d(delta_x, 4.0, RND);
            mpfr_set_d(delta_y, 4.0, RND);
            mpfr_div_ui(delta_x, delta_x, intpow(2, z), RND);
            mpfr_div_ui(delta_y, delta_y, intpow(2, z), RND);
            
            // area to calculate
            mpfr_t x_min, y_min;
            mpfr_init2(x_min, PREC);
            mpfr_init2(y_min, PREC);
            mpfr_div_d(x_min, delta_x, 2.0, RND);
            mpfr_div_d(y_min, delta_y, 2.0, RND);
            mpfr_sub(x_min, x, x_min, RND);
            mpfr_sub(y_min, y, y_min, RND);

            // render the image
            cv::Mat img = render_image_mpfr(x_min, y_min, delta_x, delta_y, res, res);

            // save the image
            char filename[100];
            sprintf(filename, "%sz%d.png", dir, z);
            imwrite(filename, img);
        }
    }//TODO
    /** else if (p == 1) {
        // create folder
        fs::create_directory("tiles/");
        for (int z = z_min; z <= z_max; z++) {  // zoom
            // create subfolder
            char dir[50];
            sprintf(dir, "tiles/z%d/", z);
            fs::create_directory(dir);

            for (int it = 0; it < intpow(2, z); it++) {      // tiles rows
                for (int jt = 0; jt < intpow(2, z); jt++) {  // tiles colums
                    // delta in real and imaginary part
                    mpfr_t delta_x = 4. / (mpfr_t)(intpow(2, z));
                    mpfr_t delta_y = 4. / (mpfr_t)(intpow(2, z));

                    // area to calculate
                    mpfr_t x_min = -2. + delta_x * it;
                    mpfr_t y_min = -2. + delta_y * jt;

                    // print information on the current rendering region
                    printf("z=%d   it=%d   jt=%d   min_re=%Lg   min_im=%Lg\n",
                           z, it, jt, x_min, y_min);

                    // render the image
                    cv::Mat img =
                        render_image(x_min, y_min, delta_x, delta_y, res, res);

                    // save the image
                    char filename[100];
                    sprintf(filename, "%s%d_%d.png", dir, it, jt);
                    imwrite(filename, img);
                }
            }
        }
    } else if (p == 2) {
        // calculate the corresponding point from the image
        std::complex<mpfr_t> c = calculate_point_from_zoom_image(
            z_min, x, y, pixel_x, pixel_y, res, res);

        // print the point
        printf("The point is -x %.50Lf -y %.50Lf\n", c.real(), c.imag());
    }**/

    // return
    return 0;
}
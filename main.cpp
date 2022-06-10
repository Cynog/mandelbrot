#include <omp.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
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
    long double x = -0.462160252884326494537958751607,
                y = -0.582399837100775696896448607731;
    int res = 1024;
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
                x = strtold(optarg, NULL);
                break;
            case 'y':
                y = strtold(optarg, NULL);
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
    std::cout << "x " << x << std::endl;
    std::cout << "y " << y << std::endl;
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
            // delta in real and imaginary part
            long double delta_x = 4. / (long double)(intpow(2, z));
            long double delta_y = 4. / (long double)(intpow(2, z));

            // area to calculate
            long double x_min = x - delta_x / 2.0;
            long double y_min = y - delta_y / 2.0;

            // print information on the current rendering region
            printf("z=%d\n", z);

            // render the image
            cv::Mat img =
                render_image(x_min, y_min, delta_x, delta_y, res, res);

            // save the image
            char filename[100];
            sprintf(filename, "%sz%d.png", dir, z);
            imwrite(filename, img);
        }
    } else if (p == 1) {
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
                    long double delta_x = 4. / (long double)(intpow(2, z));
                    long double delta_y = 4. / (long double)(intpow(2, z));

                    // area to calculate
                    long double x_min = -2. + delta_x * it;
                    long double y_min = -2. + delta_y * jt;

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
        std::complex<long double> c = calculate_point_from_zoom_image(
            z_min, x, y, pixel_x, pixel_y, res, res);

        // print the point
        printf("The point is -x %.50Lf -y %.50Lf\n", c.real(), c.imag());
    }

    // return
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main(void) {
    int res = 256;

    // load image
    char dir[50] = "zoom/";
    char filename[100];
    sprintf(filename, "%scuda.dat", dir);

    FILE *file = fopen(filename, "r");
    uint8_t *img = (uint8_t *)malloc(res * res * sizeof(uint8_t));
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < res; j++) {
            fscanf(file, "%d ", &img[i + res * j]);
        }
    }

    fclose(file);

    // convert to opencv grayscale image
    cv::Mat cvimg = cv::Mat(res, res, CV_8UC1, img);

    // apply colormap
    cv::Mat cvimg_color;
    applyColorMap(cvimg, cvimg_color, cv::COLORMAP_JET);

    // save the image
    sprintf(filename, "%scuda.png", dir);
    imwrite(filename, cvimg_color);

    // return
    return 0;
}
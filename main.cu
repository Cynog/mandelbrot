#include <unistd.h>

#include <cuComplex.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

__global__ void render_image_cuda_d(uint8_t *img, double x_min, double y_min, double delta_x, double delta_y, int res) {
    int index = threadIdx.x + blockIdx.x * blockDim.x;
    int stride = blockDim.x * gridDim.x;

    for (int ind = index; ind < res*res; ind += stride) {

    // calculate the pixels of the image
    int i = ind % res;
    int j = ind / res;
    cuDoubleComplex c = make_cuDoubleComplex(x_min + delta_x * (double)(i) / (double)(res), y_min + delta_y * (double)(j) / (double)(res));

    if (cuCabs(c) > 2) {
        img[ind] = 0;
        return;
    }

    cuDoubleComplex z = c;

    uint8_t k_write = 255;
    for (int k = 1; k < 255 * 100; k++) {
        z = cuCadd(cuCmul(z, z), c);

        if (cuCabs(z) > 2) {
            k_write = k % 255;
            break;
        }
    }

    img[ind] = k_write;
    }

    // return
    return;
}

int main(int argc, char *argv[]) {
    // parameters for the image to render
    double x_min = -2.0;
    double y_min = -2.0;
    double delta_x = 4.0;
    double delta_y = 4.0;
    int res = 256;
    int nblocks = 256;
    int nthreads = 256;

    // create folder
    char dir[50] = "zoom/";
    fs::create_directory(dir);
    // calculate the zoomed images

    // empty grayscale image
    uint8_t *img;
    size_t size = res * res * sizeof(uint8_t);
    cudaMallocManaged(&img, size);

    // render the image
    render_image_cuda_d<<<nblocks, nthreads>>>(img, x_min, y_min, delta_x, delta_y, res);

    cudaDeviceSynchronize();

    printf("Calculation finished!\n");

    // save the image
    char filename[100];
    sprintf(filename, "%scuda.dat", dir);
    
    FILE *file = fopen(filename, "w");
    for (int i=0; i<res; i++) {
        for (int j=0; j<res; j++) {
            fprintf(file, "%d ", img[i + res*j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    // return
    return 0;
}
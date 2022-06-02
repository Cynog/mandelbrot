#include <complex>
#include <fstream>
#include <iostream>

#include <opencv2/core/core.hpp>

using namespace std;

int main(void) {
    int n = 128;
    complex<long double> z, c;

    // open a file to save the calculated mandelbrot points
    FILE* file;
    file = fopen("mandelbrot.dat", "w");
    if (!file) {
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    // calculate the mandelbrot points
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c = complex<long double>(
                -2. + 4. * (long double)(i) / (long double)(n),
                -2. + 4. * (long double)(j) / (long double)(n));

            if (norm(c) > 2) {
                fprintf(file, "%d %d %d\n", i, j, 0);
                continue;
            }

            z = c;

            int k_write = 255;
            for (int k = 1; k < 255; k++) {
                z = z * z + c;

                if (norm(z) > 2) {
                    k_write = k;
                    break;
                }
            }

            fprintf(file, "%d %d %d\n", i, j, k_write);
        }
    }

    // close the file
    fclose(file);
}
#include <complex>
#include <fstream>
#include <iostream>

using namespace std;

int main(void) {
    int n = 2048;
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
            z = c;

            for (int k = 0; k < 20; k++) {
                z = z * z + c;
            }

            if (norm(z) < 2) {
                fprintf(file, "%Lf %Lf\n", c.real(), c.imag());
            }
        }
    }

    // close the file
    fclose(file);
}
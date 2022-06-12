# Mandelbrot fractal renderer
## Requirements
g++
make
opencv4
gmp library
mpfr library
## How to use
Compile using ```make```. Then run the file ```main.out``` in the following way:
```./main.out -p zoom -x -0.46 -y -0.58 -z 0:8 [-r 1024] [-d mpfr]```

```./main.out -p tiles -z 0:4 [-r 1024] [-d mpfr]```

```./main.out -p calcpointfromimage -x -0.46 -y -0.58 -z 32:32 -r 1024 -i 127:625```
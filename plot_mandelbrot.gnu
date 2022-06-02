set terminal postscript eps color
set output "mandelbrot.ps"

plot "mandelbrot.dat" u 1:2
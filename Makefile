CC = g++

PROJECT = mandelbrot.out

SRCS = mandelbrot.cpp
FLAGS = -Wall `pkg-config --cflags --libs opencv4` -fopenmp

$(PROJECT) : $(SRCS)
	$(CC) $(SRCS) -o $(PROJECT) $(FLAGS)

clean:
	rm -f *.o *.out

run:
	./mandelbrot.out
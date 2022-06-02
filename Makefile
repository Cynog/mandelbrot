CC = g++

PROJECT = mandelbrot.out

SRCS = mandelbrot.cpp
LIBS = `pkg-config --cflags --libs opencv4` -fopenmp

$(PROJECT) : $(SRCS)
	$(CC) $(SRCS) -o $(PROJECT) $(LIBS)

clean:
	rm -f *.o *.out

run:
	./mandelbrot.out
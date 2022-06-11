CXX = g++
CXXFLAGS = -Wall -O3 -lgmp -lmpfr `pkg-config --cflags --libs opencv4` -fopenmp
LIBS = mandelbrot.o
HDRS = mandelbrot.hpp


main: main.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<
	

clean:
	rm -rf *.o *.out

clean_zoom:
	rm -f zoom/*
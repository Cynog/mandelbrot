CXX = g++
CXXFLAGS = -Wall -O3 -fopenmp -lgmp `pkg-config --cflags --libs opencv4`
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
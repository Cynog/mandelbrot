CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 `pkg-config --cflags --libs mpfr opencv4` -fopenmp
NVCCFLAGS = -O3
LIBS = mandelbrot.o
HDRS = mandelbrot.hpp


main: main.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS)

cuda: main.cu
	nvcc -o $@.out $^

convert: convert.cpp
	$(CXX) -o $@.out $^ $(CXXFLAGS)
	
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<
	

clean:
	rm -rf *.o *.out

clean_zoom:
	rm -f zoom/*
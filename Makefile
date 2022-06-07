CXX = g++
CXXFLAGS = -Wall -O3 -fopenmp `pkg-config --cflags --libs opencv4`
LIBS = mandelbrot.o
HDRS = mandelbrot.hpp


main: main.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS) && ./$@.out

calculate_point: calculate_point.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS) && ./$@.out

zoom_point: zoom_point.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS) && ./$@.out

render_tiles: render_tiles.o $(LIBS)
	$(CXX) -o $@.out $^ $(CXXFLAGS) && ./$@.out


%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f *.o *.out
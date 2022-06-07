CXX = g++
FLAGS = -Wall -O3 -fopenmp `pkg-config --cflags --libs opencv4`
LIBS = mandelbrot.o
HDRS = mandelbrot.hpp


main: main.o $(LIBS)
	$(CXX) -o $@.out $^ $(FLAGS) && ./main.out

calculate_point: calculate_point.o $(LIBS)
	$(CXX) -o $@.out $^ $(FLAGS) && ./calculate_point.out

zoom_point: zoom_point.o $(LIBS)
	$(CXX) -o $@.out $^ $(FLAGS) && ./zoom_point.out

render_tiles: render_tiles.o $(LIBS)
	$(CXX) -o $@.out $^ $(FLAGS) && ./render_tiles.out

%.o: %.cpp $(HDRS)
	$(CXX) $(FLAGS) -c $<



clean:
	rm -f *.o *.out
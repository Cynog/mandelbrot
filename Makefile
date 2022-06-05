CC = g++

FLAGS = -Wall `pkg-config --cflags --libs opencv4` -fopenmp

render_tiles : render_tiles.cpp
	$(CC) render_tiles.cpp -o render_tiles.out $(FLAGS) && time ./render_tiles.out

zoom_point : zoom_point.cpp
	$(CC) zoom_point.cpp -o zoom_point.out $(FLAGS) && rm -f zoom/*.png && time ./zoom_point.out

clean:
	rm -f *.o *.out

run:
	./mandelbrot.out
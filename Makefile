OPTIMIS_FLAG	= -O3
GRAPHIC_FLAG	= -lsfml-graphics -lsfml-window -lsfml-system
EXTRA_FLAG		=

COMMON_SRC	= mandelbrot_main.cpp
DIFF_SRC	= set_pixel_color.cpp
DST			= mandelbrot

ifeq ($(IMPL), 1)
    DIFF_SRC	= set_pixel_color.cpp

else ifeq ($(IMPL), 2)
    DIFF_SRC	= set_pixel_color_buffered.cpp

else ifeq ($(IMPL), 3)
    DIFF_SRC	= set_pixel_color_intrensics.cpp
	EXTRA_FLAG 	= -march=native

endif

all:
	g++ $(EXTRA_FLAG) $(OPTIMIS_FLAG) $(COMMON_SRC) $(DIFF_SRC) -o $(DST) $(GRAPHIC_FLAG)

clean:
	rm -f *.o

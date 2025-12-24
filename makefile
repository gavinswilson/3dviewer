CC = gcc
CFLAGS = `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lm -lSDL2_gfx

TARGET = bounce
SRC = main3.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

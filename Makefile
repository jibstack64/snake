
CC = gcc
CFLAGS = -Wall -DPLATFORM_DESKTOP

LDFLAGS = -lraylib

SRC = main.c
TARGET = snake

all: $(TARGET)

windows: CC = x86_64-w64-mingw32-gcc
windows: LDFLAGS = -lraylib -lopengl32 -lgdi32 -lkernel32 -luser32 -lshell32 -lwinmm
windows: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

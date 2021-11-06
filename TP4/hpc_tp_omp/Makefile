
SRC = histogram.c
OBJ = $(SRC:.c=.o)
TARGET = $(SRC:.c=.x)

all: $(TARGET)

CFLAGS = -O0 -std=gnu11 -g -fopenmp
LDFLAGS = -lpthread -lgomp

$(OBJ): $(SRC)
	gcc $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(LDFLAGS)


.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ)



CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c11

SRC     := $(wildcard *.c)
OBJ     := $(SRC:.c=.o)
TARGET  := flasher

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

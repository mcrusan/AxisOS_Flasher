# ==============================================================================
# File:        Makefile
# Description: Build configuration for Crusan & Co. AxisOS ESP32 Flasher (axflash)
# Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
# Date:        2026110601
# Version:     v1.2.0
# ==============================================================================
# 2026110601 (MFC/PP/AGY) Added canonical 5-Field SOP Header.

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

CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11
OBJ = build
SRCS = src/main.c
TARGET = $(OBJ)/pong

all: pong_build

pong_build: $(TARGET)
$(TARGET):
	@mkdir -p build 
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -rf $(OBJ)

.PHONY: all pong_build clean
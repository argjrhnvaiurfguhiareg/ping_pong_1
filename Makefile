CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c11
OBJ = build
SRCS_STATIC = src/static/main.c
SRCS_DYNAMIC = src/dynamic/main.c
TARGET_STATIC = $(OBJ)/pong_static
TARGET_DYNAMIC = $(OBJ)/pong_dynamic

all: pong_build

pong_build: $(TARGET_STATIC) $(TARGET_DYNAMIC)

$(OBJ):
	@mkdir -p build


$(TARGET_STATIC): $(SRCS_STATIC) | $(OBJ)
	$(CC) $(CFLAGS) $(SRCS_STATIC) -o $(TARGET_STATIC)
$(TARGET_DYNAMIC): $(SRCS_DYNAMIC) | $(OBJ)
	$(CC) $(CFLAGS) $(SRCS_DYNAMIC) -o $(TARGET_DYNAMIC) -lncurses

clean:
	rm -rf $(OBJ)

.PHONY: all pong_build clean
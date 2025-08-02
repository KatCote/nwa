SRC_DIR := src
INC_DIR := inc
OBJ_DIR := build/obj
BIN_DIR := build/bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET := $(BIN_DIR)/nwa

CFLAGS := -Wall -Wextra -O2 -I$(INC_DIR)
LDFLAGS := 
LIBS := -lncursesw -lpthread -lpcap

CC := clang

.PHONY: all clean

all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
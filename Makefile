DEBUG ?= 1

TARGET = aliases

CC = gcc
CFLAGS = -Wall -Wextra
PREFIX ?= /usr/local/bin

BUILD_DIR = build
SRC_DIR = src

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES)) 

ifeq ($(shell uname -s), Darwin)
	CFLAGS += -DMACOS
endif

ifeq ($(DEBUG), 1)
	CFLAGS += -g
else
	CFLAGS += -O3
endif

all: $(TARGET)

install: $(TARGET)
	install $(BUILD_DIR)/$(TARGET) $(PREFIX)/$(TARGET)

uninstall:
	rm -rf $(PREFIX)/$(TARGET)

$(TARGET): build $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

build:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/

.PHONY : all install uninstall build clean


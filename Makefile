BIN_DIR := bin

SOURCE_FILES := src/main.cpp

CPPFLAGS := -std=c++20 -Wall -Wextra

.PHONY: all clean

all: clean
	mkdir -p $(BIN_DIR)
	$(CXX) $(CPPFLAGS) $(SOURCE_FILES) -o $(BIN_DIR)/main

clean:
	rm -rf $(BIN_DIR)

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))
TARGET := $(BIN_DIR)/markdown-parser

$(info SRCS = $(SRCS))
$(info OBJS = $(OBJS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR): 
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
	mkdir -p $(BIN_DIR)

run: all
	$(TARGET)
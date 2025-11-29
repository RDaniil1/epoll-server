CXX := clang++
CXXFLAGS := -std=c++20 -Iinclude
BUILD_DIR := build
TARGET := $(BUILD_DIR)/epoll-server

SRCS := main.cpp $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)

.PHONY: all clean

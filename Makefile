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

install: $(BUILD_DIR)/$(TARGET)
        sudo install -m 755 $(BUILD_DIR)/$(TARGET) /usr/local/bin/
        sudo install -m 644 $(TARGET).service /etc/systemd/system/
        sudo systemctl daemon-reload

clean:
	rm -r $(BUILD_DIR)

.PHONY: all install clean

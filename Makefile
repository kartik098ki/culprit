CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread
SRC_DIR = src
BUILD_DIR = build
TARGET = culprit_engine

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/server/HttpServer.cpp
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/HttpServer.o

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/core/*.hpp $(SRC_DIR)/server/*.hpp $(SRC_DIR)/tests/*.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/HttpServer.o: $(SRC_DIR)/server/HttpServer.cpp $(SRC_DIR)/server/HttpServer.hpp $(SRC_DIR)/server/JsonHelper.hpp $(SRC_DIR)/core/*.hpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/server/HttpServer.cpp -o $(BUILD_DIR)/HttpServer.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

test: $(TARGET)
	./$(TARGET) --test

run: $(TARGET)
	./$(TARGET) --serve 8080

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all test run clean

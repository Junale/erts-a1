CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -pthread -Isrc
TARGET := a3
OBJECTS := main.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

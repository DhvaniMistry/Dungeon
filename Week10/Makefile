CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lncurses
SOURCES = main.cpp Game.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = tictactoe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) $(TARGET)

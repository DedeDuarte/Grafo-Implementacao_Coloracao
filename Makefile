CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = include

SRC = src/*.cpp
TARGET = a.out

all:
	@$(CXX) $(CXXFLAGS) -I$(INCLUDES) $(SRC) -o $(TARGET)

run: all
	@./$(TARGET)
	@rm $(TARGET)

clean:
	@rm $(TARGET)
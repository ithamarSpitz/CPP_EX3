CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = catan_game
OBJS = main.o Player.o Catan.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Catan.hpp Player.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Player.o: Player.cpp Player.hpp Catan.hpp
	$(CXX) $(CXXFLAGS) -c Player.cpp

Catan.o: Catan.cpp Catan.hpp
	$(CXX) $(CXXFLAGS) -c Catan.cpp

clean:
	rm -f $(TARGET) $(OBJS)

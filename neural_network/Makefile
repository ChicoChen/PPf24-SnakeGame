CXX = g++
CXXFLAGS = -Wall -std=c++17 -O3

SRCS = test.cpp neural_network.cpp activation_func.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = test

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

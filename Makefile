CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11

SRCS = main.cpp vetor3.cpp raio.cpp esfera.cpp
OBJS = $(SRCS:.cpp=.o)

all: app

app: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MF $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f app $(OBJS)

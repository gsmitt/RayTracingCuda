CXX = g++
CXXFLAGS = -Ofast -march=native -Wall -Wextra -pedantic -std=c++14

SRCS = main.cpp vetor3.cpp raio.cpp esfera.cpp camera.cpp
OBJS = $(SRCS:.cpp=.o)

all: app

app: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $< -MF $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f app $(OBJS)

png:
	./app > output_`date +'%Y%m%d_%H%M%S'`.ppm
	convert output_*.ppm output.png
	rm -f output_*.ppm

run: app
	./app > output_`date +'%Y%m%d_%H%M%S'`.ppm
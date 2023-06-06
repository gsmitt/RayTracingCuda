cmake_minimum_required(VERSION 3.0.0)
project(RayCast VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(RayCast main.cpp vetor3.cpp imagem.cpp raio.cpp camera.cpp esfera.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -Wall -Wextra")
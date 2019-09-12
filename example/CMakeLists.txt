cmake_minimum_required(VERSION 3.5)
project(t2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_DEFINITIONS( -Wall )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 11)

set(TEST_SOURCES main.cpp)


#
# OpenCV 2
#
find_package(OpenCV 2 REQUIRED)
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIR} )
SET(LIBS ${LIBS} ${OpenCV_LIBS} -lopencv_highgui)
MESSAGE(STATUS ${OpenCV_LIBS})

#
# More libraries
#
set(LIBS ${LIBS} -lm)

find_package(MRILibrary REQUIRED)
SET(LIBS ${LIBS} MRILibrary)

add_executable(TESTS ${TEST_SOURCES})
target_link_libraries(TESTS ${LIBS})


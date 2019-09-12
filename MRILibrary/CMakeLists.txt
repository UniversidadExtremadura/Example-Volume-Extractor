cmake_minimum_required(VERSION 3.5)
project(MRILibrary)
set (MRILibrary_VERSION_MAJOR 0)
set(MRILibrary_VERSION_MINOR 9)
set(MRILibrary_VERSION_SUBMINOR 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_DEFINITIONS( -Wall )
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_STANDARD 11)

set(LIB_SOURCES datatypes/texture.h analyzers/analyzer.cpp analyzers/analyzer.h analyzers/textureAnalyzer.cpp analyzers/textureAnalyzer.h datatypes/cfa.h
        datatypes/opfta.h analyzers/cfaAnalyzer.cpp analyzers/cfaAnalyzer.h analyzers/opftaAnalyzer.cpp analyzers/opftaAnalyzer.h datatypes/extractor.h
        extractors/extractor.cpp extractors/extractor.h extractors/roiExtractor.cpp extractors/roiExtractor.h extractors/voiExtractor.cpp extractors/voiExtractor.h)
set(LIB_HEADERS datatypes/texture.h analyzers/analyzer.h analyzers/textureAnalyzer.h datatypes/cfa.h datatypes/opfta.h analyzers/cfaAnalyzer.h analyzers/opftaAnalyzer.h datatypes/extractor.h extractors/extractor.h extractors/roiExtractor.h extractors/voiExtractor.h)
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

add_library(MRILibrary SHARED ${LIB_SOURCES})
target_link_libraries(MRILibrary ${LIBS})
add_executable(TESTS ${TEST_SOURCES})
target_link_libraries(TESTS MRILibrary)

install(TARGETS MRILibrary DESTINATION /usr/local/lib/MRILibrary)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/datatypes # source directory
        DESTINATION /usr/local/include/MRILibrary # target directory
        FILES_MATCHING # install only matched files
        PATTERN *.h # select header files
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/extractors # source directory
        DESTINATION /usr/local/include/MRILibrary # target directory
        FILES_MATCHING # install only matched files
        PATTERN *.h # select header files
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/analyzers # source directory
        DESTINATION /usr/local/include/MRILibrary # target directory
        FILES_MATCHING # install only matched files
        PATTERN *.h # select header files
)

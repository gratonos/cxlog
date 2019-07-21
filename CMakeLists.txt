cmake_minimum_required (VERSION 3.5)

project (cxlog)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set (SOURCES
  src/logger.cpp
  src/logger/config.cpp
  src/logger/filter.cpp
  src/logger/logic.cpp
  )

include_directories (include)

add_library (cxlog ${SOURCES})

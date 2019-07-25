cmake_minimum_required (VERSION 3.5)

project (cxlog)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set (SOURCES
    src/logger.cpp

    src/logger/error_handler.cpp
    src/logger/filter.cpp
    src/logger/logger.cpp
    src/logger/logic.cpp

    src/formatter/null.cpp

    src/writer/null.cpp
    )

include_directories (include)

add_library (cxlog ${SOURCES})

add_subdirectory(3rdparty/fmt EXCLUDE_FROM_ALL)

target_link_libraries (cxlog fmt-header-only)

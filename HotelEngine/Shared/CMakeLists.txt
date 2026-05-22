project(HotelClientCpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs/Shared/")

set(SHARED_SOURCE_FILES Data.cpp UUID.cpp)

add_library(SharedComps ${SHARED_SOURCE_FILES})

message("Built: SharedComps")


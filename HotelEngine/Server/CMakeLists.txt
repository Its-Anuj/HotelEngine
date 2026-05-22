project(HotelEngineServer VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs/Server")

set(ASIO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libs/Asio/include")

set(asio_FOUND TRUE)
add_library(asio::asio INTERFACE IMPORTED)
target_include_directories(asio::asio INTERFACE ${ASIO_INCLUDE_DIR})

add_subdirectory(Libs/Crow)

add_executable(HotelEngineServer Server.cpp)

include_directories("Libs/Crow/include/")

target_link_libraries(HotelEngineServer PRIVATE Crow ws2_32 mswsock  SharedComps) 

message("Built: Server")

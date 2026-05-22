project(HotelClientCpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Libs/ClientCpp/")

add_subdirectory(Libs/HttpLib)
add_subdirectory(Libs/Glfw)
add_subdirectory(Libs/Nhlomann)
add_subdirectory(Libs/SqlLite)

add_executable(HotelClientCpp ClientCpp.cpp Src/Window.cpp Src/DataBase.cpp)

set(IMGUI_SOURCE_FILES Libs/ImGui/imgui.cpp  
        Libs/ImGui/backends/imgui_impl_glfw.cpp 
        Libs/ImGui/backends/imgui_impl_opengl3.cpp 
        Libs/ImGui/imgui_demo.cpp 
        Libs/ImGui/imgui_draw.cpp 
        Libs/ImGui/imgui_tables.cpp 
        Libs/ImGui/imgui_widgets.cpp)

add_library(ImGui_Opengl3 ${IMGUI_SOURCE_FILES})

include_directories("Libs/HttpLib/")
include_directories("../Shared/")
include_directories("Libs/ImGui/")
include_directories("Libs/SqlLite/include/")
include_directories("Libs/ImGui/backends/")
include_directories("Libs/Glfw/include/")
include_directories("Libs/Nhlomann/include/nlohmann")

target_link_libraries(HotelClientCpp PRIVATE nlohmann_json ws2_32 glfw ImGui_Opengl3 SQLiteCpp SharedComps opengl32 ) 

target_compile_definitions(HotelClientCpp PRIVATE
    _WIN32_WINNT=0x0A00
    WINVER=0x0A00
)

message("Built: ClientCpp")


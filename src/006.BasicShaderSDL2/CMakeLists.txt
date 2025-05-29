project(BasicShaderSDL2)

cmake_minimum_required(VERSION 3.14)

# Set the source files
set(SOURCES src/main.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})


if (EMSCRIPTEN)
    message(STATUS "Building for Emscripten...")
    add_definitions(-D_EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_WEBGL2=1 -s FULL_ES3=1")
else()
    message(STATUS "Building for native platform...")
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)

    include_directories(${GLEW_INCLUDE_DIRS})

    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} OpenGL::GL)
endif()


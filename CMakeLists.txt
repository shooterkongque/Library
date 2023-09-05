cmake_minimum_required(VERSION 3.24)
project(Library)

set(CMAKE_CXX_STANDARD 17)



aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

include_directories(${PROJECT_SOURCE_DIR}/include "C:/Program Files/MySQL/MySQL Server 8.0/include")

link_directories("C:/Program Files/MySQL/MySQL Server 8.0/lib")

link_libraries(libmysql)

add_executable(app ${SRC})

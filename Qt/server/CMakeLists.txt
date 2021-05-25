project(qt_test_dan LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Network)

set (SRCS
  client.cpp
  server.cpp)

add_executable(qt_server
  main.cpp
  ${SRCS}
)

target_link_libraries(qt_server Qt5::Core Qt5::Network)

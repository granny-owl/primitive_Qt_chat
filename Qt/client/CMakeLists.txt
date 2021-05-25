project(qt_test_dan LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core)
find_package(Qt5Network)

add_executable(qt_client
  main.cpp
  client.cpp
)
target_link_libraries(qt_client Qt5::Core Qt5::Network)

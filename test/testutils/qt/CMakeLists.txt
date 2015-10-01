set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

add_library(qttestutils gui_tester.cc qt_utils.cc)
target_link_libraries(qttestutils testutils Qt5::Core Qt5::Widgets Qt5::Test)

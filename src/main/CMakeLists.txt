# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(COMMON_LIBS snailcore qtuiview Qt5::Widgets)

add_executable(snail main.cc)
target_link_libraries(snail ${COMMON_LIBS})

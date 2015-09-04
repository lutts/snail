add_subdirectory(utils)
add_subdirectory(pfmvp)
add_subdirectory(core)
add_subdirectory(qtui)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

#add_executable(snail main.cc)
#target_link_libraries(snail snailcore qtuicore Qt5::Widgets)

add_subdirectory(qtpropertybrowser)

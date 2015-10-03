# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(COMMON_LIBS snailcore qtuiview Qt5::Widgets)

#add_executable(undo_redo_preview undo_redo_preview.cc)
#target_link_libraries(undo_redo_preview ${COMMON_LIBS})

add_executable(main_window_preview main_window_preview.cc)
target_link_libraries(main_window_preview ${COMMON_LIBS})

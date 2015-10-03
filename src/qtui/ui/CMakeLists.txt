set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qtuiview_RESOURCES
  images.qrc
  )

qt5_add_resources(qtuiview_GEN_RCS ${qtuiview_RESOURCES})

SET(qtuiview_SRCS
  main_window.cc
  main_window_view_factory.cc
  workspace_view.cc
  workspace_view_factory.cc
  work_view.cc
  work_view_factory.cc
  work_name_input_dialog.cc
  )

set(SUBDIRS)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(qtuiview_SRCS ${qtuiview_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(qtuiview SHARED ${qtuiview_SRCS} ${qtuiview_GEN_RCS})
target_link_libraries(qtuiview qtuicore Qt5::Widgets QSCore pfmvp ${Boost_LIBRARIES} pthread)

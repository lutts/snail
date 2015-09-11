add_subdirectory(lib)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

find_package(Boost REQUIRED system thread)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qtui_MOC_HDRS
  qt_uiengine.h
  main_window.h
  )

set(qtui_RESOURCES
  images.qrc
  )

qt5_add_resources(qtui_GEN_RCS ${qtui_RESOURCES})
qt5_wrap_cpp(qtui_GEN_MOC_SRCS ${qtui_MOC_HDRS})

SET(qtui_SRCS
  qt_uiengine.cc
  main_window.cc
  main_window_view_factory.cc
  workspace_presenter.cc
  workspace_view.cc
  workspace_view_factory.cc
  work_basic_info_qmodel.cc
  work_view.cc
  work_view_factory.cc
  work_name_input_dialog.cc
  candidate_item_qmodel_adapter.cc
  attribute_collection_presenter.cc
  attribute_collection_qmodel.cc
  attribute_delegate.cc
  )

set(SUBDIRS)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(qtui_SRCS ${qtui_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(qtuicore SHARED ${qtui_SRCS} ${qtui_GEN_MOC_SRCS} ${qtui_GEN_RCS})
target_link_libraries(qtuicore Qt5::Widgets qtuilib pfmvp ${Boost_LIBRARIES} pthread)
set_target_properties(qtuicore PROPERTIES COMPILE_FLAGS "-DBOOST_LOG_DYN_LINK")

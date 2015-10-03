set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qtui_RESOURCES
  images.qrc
  )

qt5_add_resources(qtui_GEN_RCS ${qtui_RESOURCES})

SET(qtui_SRCS
  qt_uiengine.cc
  main_window.cc
  main_window_view_factory.cc
  workspace_presenter.cc
  workspace_view.cc
  workspace_view_factory.cc
  work_presenter.cc
  work_view.cc
  work_view_factory.cc
  work_name_input_dialog.cc
  candidate_item_qmodel_adapter.cc
  attribute_collection_presenter.cc
  attribute_collection_qmodel.cc
  attribute_delegate.cc
  kbnode_attribute_edit_presenter.cc
  kbnode_tree_qmodel.cc
  kbnode_provider_presenter.cc
  )

set(SUBDIRS)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(qtui_SRCS ${qtui_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(qtuicore SHARED ${qtui_SRCS} ${qtui_GEN_RCS})
target_link_libraries(qtuicore Qt5::Widgets QSCore pfmvp ${Boost_LIBRARIES} pthread)
#set_target_properties(qtuicore PROPERTIES COMPILE_FLAGS "-DBOOST_LOG_DYN_LINK")

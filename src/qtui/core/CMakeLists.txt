set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Core REQUIRED)
add_definitions(-DQT_NO_CAST_FROM_ASCII)

SET(qtuicore_SRCS
  qt_uiengine.cc
  workspace_presenter.cc
  work_presenter.cc
  attribute_set_presenter.cc
  attribute_set_layout.cc
  kbnode_attribute_edit_presenter.cc
  tree_item_qmodel_impl.cc
  simple_kbnode_adder_presenter.cc
#  kbnode_link_attribute_inline_editor_presenter.cc
#  kbnode_link_attribute_popup_editor_presenter.cc
  )

set(SUBDIRS)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(qtuicore_SRCS ${qtuicore_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(qtuicore ${qtuicore_SRCS} ${qtuicore_GEN_RCS})
target_link_libraries(qtuicore Qt5::Core QSCore pfmvp ${Boost_LIBRARIES} pthread)

set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS "--suppressions=/home/lutts/lib/Qt5_debug/Qt5.3.2_debug_minimal_valgrind_suppression.supp")

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
add_definitions(-DQT_NO_CAST_FROM_ASCII)

SET(qt_RCS_ images)

foreach(rcs ${qt_RCS_})
SET(qt_RCS "${qt_RCS}Q_INIT_RESOURCE(${rcs});")
endforeach(rcs)

configure_file (
  "testconfig.h.in"
  "testconfig.h"
  )

set(TEST_COMMON_LIBS ${TEST_COMMON_LIBS} Qt5::Core)

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/qtui/core)

add_gmock_unit_test(qt_uiengine
  LIBS Qt5::Widgets)

add_gmock_unit_test(main_window_presenter)
add_gmock_unit_test(workspace_presenter)
add_gmock_unit_test(work_presenter
  LIBS pfmvp ${Boost_LIBRARIES})

add_gmock_unit_test(attribute_set_presenter)

add_gmock_unit_test(attribute_set_layout
  ${PROJECT_SOURCE_DIR}/test/core/attribute_test_stub.cc
  LIBS qttestutils Qt5::Widgets QSCore
  NO_GMOCK_MAIN)

add_gmock_unit_test(kbnode_attribute_edit_presenter)

add_gmock_unit_test(tree_item_qmodel
  ${UNDER_TEST_SOURCE_DIR}/tree_item_qmodel_impl.cc
  LIBS qttestutils)

add_gmock_unit_test(simple_kbnode_adder_presenter)
add_gmock_unit_test(kbnode_link_attribute_inline_editor_presenter)
add_gmock_unit_test(kbnode_link_attribute_popup_editor_presenter)

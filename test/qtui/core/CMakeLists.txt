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

# set(UITEST_COMMON_LIBS Qt5::Widgets qtuicore qttestutils)

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/qtui/core)

add_gmock_unit_test(qt_uiengine
  LIBS Qt5::Widgets)

add_gmock_unit_test(main_window_presenter
  LIBS Qt5::Core testutils)

add_gmock_unit_test(workspace_presenter
  LIBS Qt5::Core testutils)

add_gmock_unit_test(work_presenter
  LIBS Qt5::Core pfmvp ${Boost_LIBRARIES} testutils)

add_gmock_unit_test(attribute_set_presenter
  LIBS testutils)

add_gmock_unit_test(attribute_set_layout
  LIBS testutils qttestutils Qt5::Widgets QSCore
  NO_GMOCK_MAIN)

add_gmock_unit_test(attribute_delegate
  LIBS Qt5::Widgets testutils)

add_gmock_unit_test(kbnode_attribute_edit_presenter
  LIBS Qt5::Core testutils)

add_gmock_unit_test(kbnode_tree_qmodel
  LIBS Qt5::Core testutils qttestutils)

add_gmock_unit_test(kbnode_provider_presenter
  LIBS Qt5::Core testutils)

add_gmock_unit_test(kbnode_link_attribute_inline_editor_presenter
  LIBS Qt5::Core testutils)
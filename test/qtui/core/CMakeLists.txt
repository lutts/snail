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

add_gmock_test(qt_uiengine_test
  LIBS Qt5::Widgets qtuicore)

add_gmock_test(main_window_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(workspace_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(work_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(candidate_item_qmodel_adapter_test
  LIBS qtuicore testutils qttestutils)

add_gmock_test(attribute_collection_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(attribute_collection_qmodel_test
  LIBS qtuicore testutils qttestutils)

add_gmock_test(attribute_delegate_test
  LIBS qtuicore testutils)

add_gmock_test(kbnode_attribute_edit_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(kbnode_tree_qmodel_test
  LIBS qtuicore testutils qttestutils)

add_gmock_test(kbnode_provider_presenter_test
  LIBS qtuicore testutils)

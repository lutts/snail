set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS "--suppressions=/home/lutts/lib/Qt5_debug/Qt5.3.2_debug_minimal_valgrind_suppression.supp")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
add_definitions(-DQT_NO_CAST_FROM_ASCII)

SET(CMAKE_AUTOMOC OFF)

SET(qt_RCS_ images)

foreach(rcs ${qt_RCS_})
SET(qt_RCS "${qt_RCS}Q_INIT_RESOURCE(${rcs});")
endforeach(rcs)

configure_file (
  "testconfig.h.in"
  "testconfig.h"
  )

# set(UITEST_COMMON_LIBS Qt5::Widgets qtuicore qttestutils)

qt5_wrap_cpp(qt_uiengine_test_mocs qt_uiengine_test.cc)
add_custom_target(moc_qt_uiengine_test DEPENDS ${qt_uiengine_test_mocs})
add_gmock_test(qt_uiengine_test
  LIBS Qt5::Widgets qtuicore)
add_dependencies(qt_uiengine_test moc_qt_uiengine_test)

add_gmock_test(main_window_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(workspace_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(work_basic_info_qmodel_test
  LIBS qtuicore testutils Qt5::Test)

add_gmock_test(work_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(candidate_item_qmodel_adapter_test
  LIBS qtuicore testutils qttestutils)

add_gmock_test(work_attribute_presenter_test
  LIBS qtuicore testutils)

add_gmock_test(attr_create_view_args_test
  LIBS qtuicore testutils)

add_gmock_test(attribute_layout_test
  LIBS qtuicore testutils qttestutils)

add_subdirectory(preview)

include(xTestUtil)

INIT_QGTEST()

set(COMMON_LIBS snailcore testutils utils)

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/core)

add_gmock_unit_test(main_window_model
  LIBS testutils)

add_gmock_unit_test(workspace_model
  LIBS testutils)

add_gmock_unit_test(work_model
  LIBS testutils)

add_gmock_unit_test(work
  LIBS testutils)

add_gmock_unit_test(attribute_set_model
  LIBS testutils)

add_gmock_unit_test(generic_attribute_supplier
  LIBS testutils)

add_gmock_unit_test(kbnode_attribute_model
  LIBS testutils)

add_gmock_unit_test(simple_kbnode_adder_model
  LIBS testutils)

add_gmock_unit_test(kbnode_item_provider
  LIBS testutils)

add_gmock_unit_test(kbnode_manager
  ${UNDER_TEST_SOURCE_DIR}/kbnode_item_provider.cc
  LIBS testutils)

add_gmock_unit_test(kbnode_attribute
  LIBS testutils)

add_gmock_unit_test(kbnode_link_attribute_inline_editor_model
  LIBS testutils)

add_gmock_unit_test(kbnode_link_attribute_popup_editor_model
  ${UNDER_TEST_SOURCE_DIR}/kbnode_attribute.cc
  ${UNDER_TEST_SOURCE_DIR}/link_type.cc
  LIBS testutils)

include(xTestUtil)

INIT_QGTEST()

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/core)

add_gmock_unit_test(main_window_model)
add_gmock_unit_test(workspace_model)
add_gmock_unit_test(work_model)
add_gmock_unit_test(work)
add_gmock_unit_test(attribute_set_model
  ${PROJECT_SOURCE_DIR}/test/core/attribute_test_stub.cc)

add_gmock_unit_test(generic_attribute_supplier)
add_gmock_unit_test(kbnode_attribute_model)
add_gmock_unit_test(simple_kbnode_adder_model)
add_gmock_unit_test(kbnode_item_provider)

add_gmock_unit_test(kbnode_manager
  ${UNDER_TEST_SOURCE_DIR}/kbnode_item_provider.cc)

add_gmock_unit_test(kbnode_attribute)
add_gmock_unit_test(kbnode_link_attribute_inline_editor_model)
add_gmock_unit_test(kbnode_link_attribute_popup_editor_model)
add_gmock_unit_test(kbnode_link_attribute)
add_gmock_unit_test(link_type
  ${PROJECT_SOURCE_DIR}/test/core/attribute_test_stub.cc)

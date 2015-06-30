include(xTestUtil)

INIT_QGTEST()

set(COMMON_LIBS snailcore testutils utils)

add_gmock_test(main_window_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(workspace_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(work_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(work_test
  LIBS ${COMMON_LIBS})

add_gmock_test(attribute_adder_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(few_entity_attr_editor_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(candidate_item_test
  LIBS testutils utils)

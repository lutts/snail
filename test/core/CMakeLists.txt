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

add_gmock_test(candidate_item_test
  LIBS testutils utils)

add_gmock_test(candidate_item_any_data_test
  LIBS testutils utils)

add_gmock_test(attribute_collection_model_test
  LIBS ${COMMON_LIBS})

add_gmock_test(generic_attribute_supplier_test
  LIBS ${COMMON_LIBS})

add_gmock_test(generic_attribute_test
  LIBS ${COMMON_LIBS})

add_gmock_test(kbnode_attribute_model_test
  LIBS ${COMMON_LIBS})
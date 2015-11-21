include(xTestUtil)

INIT_QGTEST()

set(COMMON_LIBS pfmvp utils)

add_gmock_test(pf_triad_manager_test
  LIBS ${COMMON_LIBS})

add_gmock_test(pf_triad_manager_auto_remove_child_test
  LIBS ${COMMON_LIBS})

add_gmock_test(pf_triad_manager_one_model_multi_view_factory_test
  LIBS ${COMMON_LIBS})

add_gmock_test(pf_view_factory_manager_test
  LIBS ${COMMON_LIBS})

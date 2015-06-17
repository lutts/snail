include(xTestUtil)

INIT_QGTEST()

set(COMMON_LIBS pfmvp testutils utils)

add_gmock_test(pf_triad_manager_test
  LIBS ${COMMON_LIBS})

add_gmock_test(pf_view_factory_manager_test
  LIBS ${COMMON_LIBS})

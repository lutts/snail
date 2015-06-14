include(xTestUtil)

INIT_QGTEST()

set(COMMON_LIBS snailcore testutils utils)

add_gmock_test(pf_triad_manager_test
  LIBS ${COMMON_LIBS})

add_subdirectory(command)

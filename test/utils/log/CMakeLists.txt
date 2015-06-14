include(xTestUtil)

INIT_QGTEST()

add_gmock_test(log_test
  LIBS testutils utils)

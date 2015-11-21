include(xTestUtil)

INIT_QGTEST()

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/utils/log)

add_gmock_unit_test(log)

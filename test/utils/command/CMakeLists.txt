set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/utils/command)

add_gmock_unit_test(command_processor)

add_gmock_unit_test(relay_command
  LIBS testutils)

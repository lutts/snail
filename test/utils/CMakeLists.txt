include(xTestUtil)

INIT_QGTEST()

set(UNDER_TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/utils)

add_gmock_unit_test(singleton)

add_gmock_test(enum_class_range_test
  LIBS utils)

add_gmock_test(range_test
  LIBS utils)

add_gmock_test(model_view_mock_generator_test)

add_gmock_test(mock_object_generator_test)

add_gmock_test(qmodelindex_generator_test
  LIBS utils Qt5::Core)

add_executable(boost_signal2_wp_memory_leak_test boost_signal2_wp_memory_leak_test.cc)
target_link_libraries(boost_signal2_wp_memory_leak_test utils)

add_gmock_test(move_test
  LIBS utils)

add_executable(testmove testmove.cc)

add_subdirectory(log)
add_subdirectory(command)

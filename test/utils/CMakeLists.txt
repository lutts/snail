include(xTestUtil)

INIT_QGTEST()

#find_package(Qt5Core)
#add_definitions(-DQT_NO_CAST_FROM_ASCII)

add_gmock_test(singleton_test
  LIBS utils)

add_gmock_test(enum_class_range_test
  LIBS utils)

add_gmock_test(range_test
  LIBS utils)

add_gmock_test(model_view_mock_generator_test)

add_gmock_test(mock_object_generator_test)

add_subdirectory(log)

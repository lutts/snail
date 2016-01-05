include(xTestUtil)

INIT_QGTEST()

add_gmock_test(loki_abstract_factory_test)
set_source_files_properties(loki_abstract_factory_test.cc PROPERTIES COMPILE_FLAGS -Wno-overloaded-virtual)

add_library(lokiDemoLib loki_demo_lib.cc)
set_source_files_properties(loki_demo_lib.cc PROPERTIES COMPILE_FLAGS -Wno-overloaded-virtual)

add_executable(lokiDemo loki_demo.cc)
target_link_libraries(lokiDemo lokiDemoLib)
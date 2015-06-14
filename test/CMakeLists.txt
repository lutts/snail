include(xTestUtil)

INIT_QGTEST()

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/gmock_ext)

add_subdirectory(testutils)

add_subdirectory(utils)
add_subdirectory(core)
add_subdirectory(qtui)


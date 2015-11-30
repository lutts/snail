include(xTestUtil)


INIT_QGTEST()

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/gmock_ext)

add_subdirectory(testutils)

set(TEST_COMMON_LIBS testutils)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")

add_subdirectory(utils)
add_subdirectory(pfmvp)
add_subdirectory(core)
add_subdirectory(qtui)

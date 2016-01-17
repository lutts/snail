# i would to see more projects using these defaults
#ADD_DEFINITIONS( "-Wall" )
# ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

MACRO(INIT_QGTEST)
  find_package(GTest REQUIRED)
  find_package(GMock REQUIRED)
  find_package(Qt5Test REQUIRED)
  SET(USE_GMOCK TRUE)
  #find_package(CMakeParseArguments)
  include_directories(SYSTEM
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS})
ENDMACRO(INIT_QGTEST)

#
# Qt-Google Test helper function
#
# Prerequist:
#   users must add `find_package(CMakeParseArguments)` before use this macro
#   one can use INIT_QGTEST to do this
#
# parameters:
#   EXT_MOCS: external mocs
#   INC_MOCS: include mocs
#   SRCS: other files which do not need moc
#   LIBS: libs to link with, Qt5::Test, GTest, GMock modules
#         automatically added, so do not need to list in LIBS
#
function(add_qg_test testname)
  set(options DONOT_AUTORUN)
  set(multiValueArgs EXT_MOCS INC_MOCS SRCS LIBS)
  cmake_parse_arguments(QGTEST "${options}" "" "${multiValueArgs}" ${ARGN})

  # message("ARGN = ${ARGN}")
  # message("QGTEST_EXT_MOCS = ${QGTEST_EXT_MOCS}")
  # message("QGTEST_INC_MOCS = ${QGTEST_INC_MOCS}")
  # message("QGTEST_SRCS = ${QGTEST_SRCS}")
  # message("QGTEST_LIBS = ${QGTEST_LIBS}")

  # set(QGTEST_SRCS ${QGTEST_SRCS} ${GUI_TESTER_IMPL_PATH})
  # set(QGTEST_EXT_MOCS ${QGTEST_EXT_MOCS} ${GUI_TESTER_HEADER_PATH})

  qt5_wrap_cpp(${testname}_INC_MOCS ${QGTEST_INC_MOCS})
  qt5_wrap_cpp(${testname}_EXT_MOCS ${QGTEST_EXT_MOCS})

  set(QGTEST_MOCS ${QGTEST_INC_MOCS} ${QGTEST_EXT_MOCS} ${${testname}_EXT_MOCS})

  add_custom_target(moc_${testname}_target DEPENDS ${${testname}_INC_MOCS})
  add_executable(${testname} ${QGTEST_MOCS} ${QGTEST_SRCS})
  add_dependencies(${testname} moc_${testname}_target)

  if (${USE_GMOCK})
    target_link_libraries(${testname} ${QGTEST_LIBS} ${GMOCK_LIBRARIES} Qt5::Test pthread ${TEST_COMMON_LIBS})
  else(${USE_GMOCK})
    target_link_libraries(${testname} ${QGTEST_LIBS} ${GTEST_LIBRARIES} Qt5::Test pthread ${TEST_COMMON_LIBS})
  endif(${USE_GMOCK})

  # message("${CMAKE_MEMORYCHECK_COMMAND}")
  # message("${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")

  add_test(${testname} ${testname})
  add_test(qtui_memcheck_${testname}
    ${CMAKE_MEMORYCHECK_COMMAND}
    ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}
    ${CMAKE_CURRENT_BINARY_DIR}/${testname})

  if (NOT ${QGTEST_DONOT_AUTORUN})
    add_custom_command(TARGET ${testname}
      POST_BUILD
      COMMAND ${testname}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Running ${target}" VERBATIM)
  endif(NOT ${QGTEST_DONOT_AUTORUN})
endfunction(add_qg_test)

function(add_gmock_test target)
  set(options DONOT_AUTORUN NO_GMOCK_MAIN)
  set(oneValueArgs TESTSUITE)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(QGTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # message("LIBS = ${QGTEST_LIBS}")
  # message("UNPARSED_ARGUMENTS = ${QGTEST_UNPARSED_ARGUMENTS}")

  set(QGTEST_SRCS ${QGTEST_UNPARSED_ARGUMENTS})

  if ("${QGTEST_SRCS}" STREQUAL "")
    set(QGTEST_SRCS ${target}.cc)
  endif()

  if ("${QGTEST_TESTSUITE}" STREQUAL "")
    set(QGTEST_TESTSUITE all_tests)
  endif()

  add_executable(${target} ${QGTEST_SRCS})

  if (TARGET ${QGTEST_TESTSUITE})
    add_dependencies(${QGTEST_TESTSUITE} ${target})
  endif()

  if (NOT ${QGTEST_NO_GMOCK_MAIN})
    target_link_libraries(${target} ${QGTEST_LIBS} ${GMOCK_BOTH_LIBRARIES} pthread ${TEST_COMMON_LIBS})
  else(NOT ${QGTEST_NO_GMOCK_MAIN})
    target_link_libraries(${target} ${QGTEST_LIBS} ${GMOCK_LIBRARIES} pthread ${TEST_COMMON_LIBS})
  endif(NOT ${QGTEST_NO_GMOCK_MAIN})

  # message("${CMAKE_MEMORYCHECK_COMMAND}")
  # message("${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}")
  add_test(${target} ${target})
  add_test(memcheck_${target}
    ${CMAKE_MEMORYCHECK_COMMAND}
    ${CMAKE_MEMORYCHECK_COMMAND_OPTIONS}
    ${CMAKE_CURRENT_BINARY_DIR}/${target})

  if (NOT ${QGTEST_DONOT_AUTORUN})
    add_custom_command(TARGET ${target}
      POST_BUILD
      COMMAND ${target}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Running ${target}" VERBATIM)
  endif(NOT ${QGTEST_DONOT_AUTORUN})
endfunction(add_gmock_test)

function(add_gmock_unit_test target)
  set(TEST_SRCS ${target}_test.cc)
  if (EXISTS ${UNDER_TEST_SOURCE_DIR}/${target}.cc)
    set(TEST_SRCS ${TEST_SRCS} ${UNDER_TEST_SOURCE_DIR}/${target}.cc)
  endif()
  add_gmock_test(${target}_test
    ${TEST_SRCS}
    ${ARGN})
endfunction(add_gmock_unit_test)

# http://snikt.net/index.php/2010/04/01/howto-use-cmake-with-cc-projects
MACRO(find_doxygen)
  # check if doxygen is even installed
  find_package(Doxygen) # REQUIRED)

  if(EXISTS ${DOXYGEN_EXECUTABLE})
    # prepare doxygen configuration file
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    # add doxygen as target
    add_custom_target(doxygen ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    # cleanup $build/api-doc on "make clean"
    set_property(DIRECTORY APPEND PROPERTY
      ADDITIONAL_MAKE_CLEAN_FILES api-doc)

    # add doxygen as dependency to doc-target
    get_target_property(DOC_TARGET doc TYPE)
    if(NOT DOC_TARGET)
      add_custom_target(doc)
    endif()
    add_dependencies(doc doxygen)

    # install HTML API documentation and manual pages
    set(DOC_PATH "share/doc/${CPACK_PACKAGE_NAME}-${VERSION}")
    #
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api-doc/html
      DESTINATION ${DOC_PATH}
      )
    #
    # install man pages into packages, scope is now project root..
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api-doc/man/man3
      DESTINATION share/man/man3/
      )
  endif()
ENDMACRO()

FUNCTION(add_assembler_output _target)
  ADD_DEFINITIONS( "-g -save-temps" )
  add_custom_target(lss COMMAND objdump -h -S ${_target} >${_target}.lss DEPENDS ${_target})
  #add_custom_command(TARGET ${_target} COMMAND objdump -h -S ${_target} >${_target}.lss)
ENDFUNCTION()

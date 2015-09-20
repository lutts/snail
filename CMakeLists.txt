project(Snail)
cmake_minimum_required(VERSION 2.8.11)

# Version information
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(SNAIL_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION ${SNAIL_VERSION})
set(RELEASE_YEAR 2015)

enable_testing(true)
set(CMAKE_MEMORYCHECK_COMMAND valgrind)
set(CMAKE_MEMORYCHECK_COMMAND_OPTIONS "")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

include(CommonUtils)

# activate C++ 11
add_definitions(-std=c++11 -g)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-Wall -Wextra -Werror -Wno-error=unused-variable")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(warnings "/W4 /WX /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${warnings}"
  CACHE STRING "Flags used by the compiler during all build types." FORCE)
set(CMAKE_C_FLAGS   "${warnings}"
  CACHE STRING "Flags used by the compiler during all build types." FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

find_package(Gettext REQUIRED)
add_definitions(-DPACKAGE="${CMAKE_PROJECT_NAME}")

# boost
find_package(Boost REQUIRED locale system thread log log_setup)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_definitions(-DBOOST_LOG_DYN_LINK)

add_subdirectory(src)
add_subdirectory(test)

ADD_CPPLINT_SUPPORT("")

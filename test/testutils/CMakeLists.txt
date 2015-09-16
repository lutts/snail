find_package(Boost REQUIRED system thread log log_setup)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(testutils utils.cc log_testspy.cc user_log_testspy.cc)

add_subdirectory(qt)

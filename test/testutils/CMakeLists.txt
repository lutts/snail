find_package(Boost REQUIRED system thread log-mt log_setup)

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

add_library(testutils utils.cc log_testspy.cc user_log_testspy.cc)

add_subdirectory(qt)

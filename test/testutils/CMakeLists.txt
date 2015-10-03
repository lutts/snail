add_library(testutils utils.cc log_testspy.cc user_log_testspy.cc)
target_link_libraries(testutils ${Boost_LIBRARIES} utils)

add_subdirectory(qt)

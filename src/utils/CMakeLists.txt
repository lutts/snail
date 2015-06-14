# add boost
find_package(Boost REQUIRED locale system thread log-mt log_setup)

include_directories(${Boost_INCLUDE_DIRS})

add_library(utils
  utils.cc
  log/log.cc
)
target_link_libraries(utils ${Boost_LIBRARIES} pthread)
set_target_properties(utils PROPERTIES COMPILE_FLAGS "-DBOOST_LOG_DYN_LINK")

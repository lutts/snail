# add boost

set(SUBDIRS command)

set(utils_SRCS)
foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(utils_SRCS ${utils_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

set(utils_SRCS ${utils_SRCS}
  utils.cc
  type.cc
  log/log.cc
)

add_library(utils ${utils_SRCS})
target_link_libraries(utils ${Boost_LIBRARIES} pthread)
#set_target_properties(utils PROPERTIES COMPILE_FLAGS "-DBOOST_LOG_DYN_LINK")

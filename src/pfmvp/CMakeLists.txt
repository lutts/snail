set(SUBDIRS)

include_directories(SUBDIRS)

set(pfmvp_SRCS)
foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(pfmvp_SRCS ${pfmvp_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

set(pfmvp_SRCS ${pfmvp_SRCS}
  pf_view_factory_manager.cc
  pf_triad_manager.cc
  )

add_library(pfmvp SHARED ${pfmvp_SRCS})
target_link_libraries(pfmvp utils)

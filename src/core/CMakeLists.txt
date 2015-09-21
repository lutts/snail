set(SUBDIRS)

set(core_SRCS)
foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(core_SRCS ${core_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

set(core_SRCS ${core_SRCS}
  main_window_model.cc
  workspace_model.cc
  work_model.cc
  application_factory.cc
  attribute_collection_model.cc
  kbnode_attribute_model.cc
  )

add_library(snailcore ${core_SRCS})
target_link_libraries(snailcore utils)

# set_source_files_properties(pf_model_id_ints.cc PROPERTIES COMPILE_FLAGS "-g -save-temps")

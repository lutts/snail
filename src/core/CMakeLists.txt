set(core_SRCS ${core_SRCS}
  main_window_model.cc
  workspace_model.cc
  work.cc
  work_model.cc
  attribute_set_model.cc
  kbnode_attribute_model.cc
  simple_kbnode_adder_model.cc
  kbnode_item_provider.cc
  kbnode_manager.cc
  kbnode_attribute.cc
  kbnode_link_attribute_inline_editor_model.cc
  kbnode_link_attribute_popup_editor_model.cc
  kbnode_link_attribute.cc
  link_type.cc
  )

set(SUBDIRS factory)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(core_SRCS ${core_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(snailcore ${core_SRCS})
target_link_libraries(snailcore utils)


# set_source_files_properties(pf_model_id_ints.cc PROPERTIES COMPILE_FLAGS "-g -save-temps")

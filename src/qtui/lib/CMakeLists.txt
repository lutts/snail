find_package(Qt5Widgets REQUIRED)
add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qtuilib_MOC_HDRS
  relocatable_gridlayout.h
  )

set(qtuilib_RESOURCES
  )

qt5_add_resources(qtuilib_GEN_RCS ${qtuilib_RESOURCES})
qt5_wrap_cpp(qtuilib_GEN_MOC_SRCS ${qtuilib_MOC_HDRS})

SET(qtuilib_SRCS
  relocatable_gridlayout.cc
  layout_engine.cc
  )

set(SUBDIRS)

foreach (_subdir ${SUBDIRS})
  include(${_subdir}/Sources.cmake)
  foreach(_file ${${_subdir}_SRCS})
    set(qtuilib_SRCS ${qtuilib_SRCS} ${_subdir}/${_file})
  endforeach()
endforeach()

add_library(qtuilib STATIC ${qtuilib_SRCS} ${qtuilib_GEN_MOC_SRCS} ${qtuilib_GEN_RCS})
target_link_libraries(qtuilib Qt5::Widgets)

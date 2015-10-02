set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

qt5_add_resources(qsint_Core_RES
  ${CMAKE_CURRENT_SOURCE_DIR}/actionpanel/schemes.qrc)

set(qsint_Core_SRCS
  actionlabel.cpp
  actionpanel/actionpanel.cpp
  actionpanel/actionpanelscheme.cpp
  actionpanel/taskheader_p.cpp
  actionpanel/taskgroup_p.cpp
  actionpanel/actiongroup.cpp
  )

include_directories(actionpanel)

add_library(QSCore ${qsint_Core_SRCS} ${qsint_Core_RES})
# Use the Widgets module from Qt 5.
target_link_libraries(QSCore Qt5::Widgets)

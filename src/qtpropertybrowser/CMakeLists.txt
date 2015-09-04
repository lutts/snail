# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

find_package(Boost REQUIRED system thread)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qtpropertybrowser_MOC_HDRS
  )

set(qtpropertybrowser_RESOURCES
  qtpropertybrowser.qrc
  )

qt5_add_resources(qtpropertybrowser_GEN_RCS ${qtpropertybrowser_RESOURCES})
qt5_wrap_cpp(qtpropertybrowser_GEN_MOC_SRCS ${qtpropertybrowser_MOC_HDRS})

SET(qtpropertybrowser_SRCS
  qtpropertybrowser.cpp
  qtabstractpropertymanager.cpp
  qtpropertymanager.cpp
  qtabstracteditorfactory.cpp
  qteditorfactory.cpp
  qttreepropertybrowser.cpp
  qtbuttonpropertybrowser.cpp
  qtgroupboxpropertybrowser.cpp
  qtpropertybrowserutils.cpp
  demo.cpp
  )

set(CMAKE_AUTOMOC ON)

add_executable(qtpropertybrowser ${qtpropertybrowser_SRCS} ${qtpropertybrowser_GEN_MOC_SRCS} ${qtpropertybrowser_GEN_RCS})
target_link_libraries(qtpropertybrowser Qt5::Widgets pfmvp ${Boost_LIBRARIES} pthread)
set_target_properties(qtpropertybrowser PROPERTIES COMPILE_FLAGS "-DBOOST_LOG_DYN_LINK")

set(CMAKE_AUTOMOC OFF)

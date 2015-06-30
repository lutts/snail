find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(qttestutils_MOC_FILES
  gui_tester.h
  )

qt5_wrap_cpp(qttestutils_GEN_MOC_SRCS ${qttestutils_MOC_FILES})

qt5_generate_moc(gui_tester.cc moc_gui_tester_cc.cc)


add_custom_target(moc_qttestutils_target DEPENDS
  moc_gui_tester_cc.cc
  )

add_library(qttestutils gui_tester.cc qt_utils.cc ${qttestutils_GEN_MOC_SRCS})
target_link_libraries(qttestutils testutils Qt5::Core Qt5::Widgets Qt5::Test)
add_dependencies(qttestutils moc_qttestutils_target)

# CMakeList.txt : CMake project for ForegroundExtractionApp, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project (ForegroundExtractionApp LANGUAGES CXX)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

set (headers 
	ForegroundExtractionApp.h 
	mainwindow.h
	myimagelabel.h
	helper.h)

set (source_cpps 
	ForegroundExtractionApp.cpp 
	mainwindow.cpp
	myimagelabel.cpp)

set (uis mainwindow.ui)

qt5_wrap_ui (headers_wrapped ${uis})
qt5_wrap_cpp (source_moc ${headers})

# Add source to this pr.
add_executable (${PROJECT_NAME} ${source_cpps} ${headers_wrapped} ${source_moc})

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets)

# TODO: Add tests and install targets if needed.

include(Macros.cmake)

cmake_minimum_required(VERSION 3.25.1)

project(ChatWithGPT LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE HEADER_FILES Include/*.h)
file(GLOB_RECURSE SOURCE_FILES Source/*.cpp Source/*.h)
file(GLOB_RECURSE RESOURCE_FILES Resources/*.resx)

GENERATE_FOLDER_SG("${HEADER_FILES}" ${CMAKE_CURRENT_SOURCE_DIR}/Include Include/)
GENERATE_FOLDER_SG("${SOURCE_FILES}" ${CMAKE_CURRENT_SOURCE_DIR}/Source Source/)
GENERATE_FOLDER_SG("${PRECOMPILED_FILES}" ${CMAKE_CURRENT_SOURCE_DIR} Source/)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${PRECOMPILED_FILES} ${RESOURCE_FILES} ${ICONS_FILES})

#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
#set(Boost_NO_BOOST_CMAKE TRUE)
#set(Boost_NO_SYSTEM_PATHS TRUE) 

#if (Boost_NO_SYSTEM_PATHS)
#    set(BOOST_ROOT "c:/Tools/boost_1_82_0/" CACHE PATH "Boost library path")
#    set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
#    set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib/")    
#endif (Boost_NO_SYSTEM_PATHS)

#find_package(Qt5 REQUIRED COMPONENTS Core)
#find_package(Qt5 REQUIRED COMPONENTS Gui)
#find_package(Qt5 REQUIRED COMPONENTS Widgets)
#qt_standard_project_setup()

GET_QT_PREFIX_PATH(QT_PATH)
SET(CMAKE_PREFIX_PATH ${QT_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core QUIET)
find_package(Qt5Gui QUIET)
find_package(Qt5Widgets QUIET)
#include_directories(${Qt5Widgets_INCLUDES})
#add_definitions(${Qt5Widgets_DEFINITIONS})
#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#qt_standard_project_setup()
# find_package(Boost 1.82.0 regex date_time system filesystem thread graph program_options)
#include_directories(${BOOST_INCLUDE_DIRS})

# set (Boost_LIBRARIES asio)

TARGET_LINK_LIBRARIES( ChatWithGPT PRIVATE
 #${Boost_LIBRARIES}
 ${Qt5Widgets_LIBRARIES}
 C:/liboai-main/liboai.lib
 )
 #target_include_directories( ChatWithGPT PUBLIC C:\liboai-main\liboai\include )
target_include_directories(ChatWithGPT PUBLIC
  Include
  C:/liboai-main/liboai/include )
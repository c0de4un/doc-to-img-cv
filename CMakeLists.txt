# // ------------------------------------------------------------------------------
# 
# This is CMake-Project for "ImgToDoc-CV" an utility application
# for converting image of document to its close analog (just like scan), ofcource with
# some issues due to big angles.
# 
# License: see "LICENSE" file.
# Tools: QT || VC++, GCC || CLang || MSVC
# API: C/C++, OpenCV.
# Platforms: Any with C++
# 
# // ------------------------------------------------------------------------------

# =================================================================================
# CMake Meta-Data
# =================================================================================

# CMake-Version
cmake_minimum_required ( VERSION 3.5 FATAL_ERROR ) # QT5 + CMake required 3.5 minimum.

# =================================================================================
# PROJECT
# =================================================================================

# Project Name
set ( PROJECT_NAME "img_to_doc_cv" )

# Project Version
set ( PROJECT_VERSION 0.0.1 )

# Configure Project
project ( ${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C CXX )

# C++ Version
set ( CMAKE_CXX_STANDARD 11 ) # For campatability with legacy OS, like Windows 7 (before 2011)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Compiler-specific tunes (RC, CC, etc).
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# =================================================================================
# CONFIGS
# =================================================================================

# Prefix to headers & sources dir.
set ( PROJECT_SRC_DIR_PREFIX "src/cxx" )

# Prefix for Third-Party Source-Code (not lib's includes).
set ( PROJECT_EXTERNAL_SRC_DIR_PREFIX "external" )

# SHARED (DYNAMIC) & STATIC Libs Dir.
set ( PROJECT_LIBS_DIR "libs/lib" )

# Includes Dir for Libs.
set ( PROJECT_LIBS_INCLUDES_DIR "libs/includes" )

# Prefix to qml dir.
set ( PROJECT_QML_DIR_PREFIX "src/qml/" )

# Add Definition, that QT API used.
add_definitions ( -DQFT_USE_QT=1 )

# =================================================================================
# QT Libraries/Modules
# =================================================================================

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

#find_package(Qt5 COMPONENTS Core Quick LinguistTools Qml QuickControls2 REQUIRED)

# =================================================================================
# HEADERS
# =================================================================================

set ( ITDCV_HEADERS
    # CONFIGS
    #"${PROJECT_SRC_DIR_PREFIX}itdcv/config/numeric.hpp"
    # CORE
    "${PROJECT_SRC_DIR_PREFIX}/itdcv/win/main.hpp" )

# =================================================================================
# SOURCES
# =================================================================================

set ( ITDCV_SOURCES
    # CORE
	"${PROJECT_SRC_DIR_PREFIX}/itdcv/win/main.cpp" )

# =================================================================================
# RESOURCES
# =================================================================================

# Localizations

# =================================================================================
# BUILD
# =================================================================================

# Platform
if ( ANDROID ) # Android
	
	# Add SHARED Library for Android
    add_library ( itdcv SHARED ${ITDCV_SOURCES} ${ITDCV_HEADERS} )
	
else ( ANDROID ) # Desktop (Mac, Windows, Linux)
	
	# Add Executable-Object
    add_executable ( itdcv ${ITDCV_SOURCES} ${ITDCV_HEADERS} )
	
	# Configure Executable Object
	set_target_properties ( itdcv PROPERTIES
		OUTPUT_NAME "ImageToDocument-CV"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}" )
	
endif ( ANDROID )

# Link
#

target_compile_definitions( itdcv
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>:QT_QML_RELEASE> )

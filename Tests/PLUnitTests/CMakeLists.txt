#*********************************************************#
#*  File: CMakeLists.txt                                 *
#*
#*  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
#*  and associated documentation files (the "Software"), to deal in the Software without
#*  restriction, including without limitation the rights to use, copy, modify, merge, publish,
#*  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
#*  Software is furnished to do so, subject to the following conditions:
#*
#*  The above copyright notice and this permission notice shall be included in all copies or
#*  substantial portions of the Software.
#*
#*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
#*  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#*  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
#*  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#*********************************************************#


##################################################
## Project
##################################################
define_project(PLUnitTests NOSUFFIX)

##################################################
## Source files
##################################################
add_sources(
	src/UnitTests.cpp
	# PLCore
		# Application
		src/PLCore/Application/ApplicationContext.cpp
		src/PLCore/Application/CoreApplication.cpp
		# Config
		src/PLCore/Config.cpp
		# Container
		src/PLCore/Container/Array.cpp
		src/PLCore/Container/Bitset.cpp
		src/PLCore/Container/ContainerTests.h
		src/PLCore/Container/FastPool.cpp
		src/PLCore/Container/List.cpp
		src/PLCore/Container/Pool.cpp
		src/PLCore/Container/Queue.cpp
		src/PLCore/Container/SimpleList.cpp
		src/PLCore/Container/Stack.cpp
		# Heap
		src/PLCore/Container/BinaryHeap.cpp
		src/PLCore/Container/BinominalHeap.cpp
		src/PLCore/Container/FibonacciHeap.cpp
		src/PLCore/Container/HeapTests.h
		# Map
		src/PLCore/Container/HashMap.cpp
		src/PLCore/Container/MapTests.h
		src/PLCore/Container/SimpleMap.cpp
		# FileSystem
		src/PLCore/FileSystem.cpp
		# Log
		src/PLCore/Log.cpp
		# String
		src/PLCore/String.cpp
		# Tools
		src/PLCore/Tools/ChecksumCRC32.cpp
		src/PLCore/Tools/ChecksumMD5.cpp
		src/PLCore/Tools/ChecksumSHA1.cpp
		src/PLCore/Tools/Localization.cpp
		# others
		src/PLCore/ResourceManagement.cpp
		src/PLCore/ResourceManager.cpp
	# PLMath
		src/PLMath/Intersect.cpp
		src/PLMath/EulerAngles.cpp
		src/PLMath/Graph.cpp
		src/PLMath/GraphPath.cpp
		src/PLMath/Math.cpp
		src/PLMath/Matrix3x3.cpp
		src/PLMath/Matrix3x4.cpp
		src/PLMath/Matrix4x4.cpp
		src/PLMath/Quaternion.cpp
		src/PLMath/Vector2.cpp
		src/PLMath/Vector3.cpp
		src/PLMath/Vector4.cpp
		# UnitTest++ AddIns
		src/UnitTest++AddIns/MyMobileTestReporter.cpp
		src/UnitTest++AddIns/MyTestReporter.cpp
		src/UnitTest++AddIns/PLChecks.cpp
		src/UnitTest++AddIns/RunAllTests.cpp
		src/UnitTest++AddIns/wchar_template.cpp
		src/UnitTest++AddIns/LocaleWrapper.cpp
)
if(ANDROID)
	add_sources(
		src/MainAndroid.cpp
	)
else()
	add_sources(
		src/Main.cpp
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	include
	${UNITTESTPP_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/Base/PLCore/include
	${CMAKE_SOURCE_DIR}/Base/PLMath/include
)

##################################################
## Library directories
##################################################
add_link_directories(
	${PL_LIB_DIR}
)

##################################################
## Additional libraries
##################################################
add_libs(
	${UNITTESTPP_LIBRARIES}
	PLCore
	PLMath
)

##################################################
## Preprocessor definitions
##################################################
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_compile_defs(
		${WIN32_COMPILE_DEFS}
		_ITERATOR_DEBUG_LEVEL=0
	)
elseif(LINUX)
	##################################################
	## Linux
	##################################################
	add_compile_defs(
		${LINUX_COMPILE_DEFS}
	)
endif()

##################################################
## Compiler flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_compile_flags(
		${WIN32_COMPILE_FLAGS}
		/EHsc			# Enable exception handling for this project
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	add_compile_flags(
		${LINUX_COMPILE_FLAGS}
		-fexceptions	# Enable exception handling for this project
	)
endif()

##################################################
## Linker flags
##################################################
if(WIN32)
	##################################################
	## MSVC Compiler
	##################################################
	add_linker_flags(
		${WIN32_LINKER_FLAGS}
	)
elseif(LINUX)
	##################################################
	## GCC Compiler
	##################################################
	if(ANDROID)
		add_linker_flags(
			-L${STL_LIBRARIES_PATH}	# UnitTest++ is using stl, adding this line results in slightly bigger executables so we don't do this within "LinuxNDK.cmake"
		)
	endif()
	add_linker_flags(
		${LINUX_LINKER_FLAGS}
	)
endif()

##################################################
## Build
##################################################
if(ANDROID)
	build_library(${CMAKETOOLS_CURRENT_TARGET} SHARED)
else()
	build_executable(${CMAKETOOLS_CURRENT_TARGET}  CONSOLE)
endif()

##################################################
## Dependencies
##################################################
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	PLCore External-UnitTest++)
add_dependencies(Tests							${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

if(ANDROID)
	# Dynamic library
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB} ${PL_TESTS_BIN_DIR}
	)
else()
	# Executable
	add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_EXECUTABLE} ${PL_TESTS_BIN_DIR}
	)
endif()


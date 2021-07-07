#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.8)

#Name your project here
project(volumespider)

#set the module directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
#MESSAGE("CMAKE_SOURCE_DIR    "${CMAKE_SOURCE_DIR})
#MESSAGE("CMAKE_MODULE_PATH   "${CMAKE_MODULE_PATH})


#find cuda

#build the executable in the binary directory on MS Visual Studio
if ( MSVC )
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
endif ( MSVC )

#set(CUDA_NVCC_FLAGS "-w -pg -g")
#set(CMAKE_C_FLAGS "-w -pg -g")
#set(CMAKE_CXX_FLAGS "-w -pg -g -fpermissive")

if( CMAKE_COMPILER_IS_GNUCC )
	set(CUDA_HOST_COMPILER /usr/bin/gcc-4.8)
	set(CUDA_NVCC_FLAGS "")
	set(CMAKE_C_FLAGS "-O3")
	set(CMAKE_CXX_FLAGS "-O3")
endif( CMAKE_COMPILER_IS_GNUCC )

find_package(CUDA REQUIRED)


#find OpenCV
find_package(OpenCV REQUIRED)
add_definitions(-DUSING_OPENCV)

#find ANN
find_package(ANN REQUIRED)

#find glut
find_package(GLUT REQUIRED)

#find glew
find_package(GLEW REQUIRED)

#find OpenGL
find_package(OpenGL REQUIRED)

#find Boost for Unix-based file lists
find_package(Boost COMPONENTS filesystem system regex)

if(Boost_FILESYSTEM_FOUND)
	set(Boost_USE_STATIC_LIBS ON)
	add_definitions(-DBOOST_PRECOMPILED)
	set(-BOOST_PRECOMPILED ON)
endif(Boost_FILESYSTEM_FOUND)

#Testing statements
#add_definitions(-DDEBUG)

#Timing statements
add_definitions(-DTIMING)

#find StimLib
find_package(STIM REQUIRED)

#find the pthreads package
find_package(Threads)

#find the X11 package
find_package(X11)

#include directories
include_directories(${CUDA_INCLUDE_DIRS}
		    ${Boost_INCLUDE_DIRS}
		    ${OPENGL_INCLUDE_DIRS}
		    ${GLEW_INCLUDE_PATH}
		    ${GLUT_INCLUDE_DIR}
		    ${JPEG_INCLUDE_DIRS}
		    ${STIM_INCLUDE_DIRS}
		    ${ANN_INCLUDE_DIR}
		    )

#link_directories(${GLUT_LIBRARY_DIRS} ${OpenGL_LIBRARY_DIRS})
#add_definitions(${GLUT_DEFINITIONS} ${OpenGL_DEFINITIONS})

#Assign source files to the appropriate variables
file(GLOB SRC_CPP "*.cpp")
file(GLOB SRC_H "*.h")
#file(GLOB SRC_CU "TextureTest_Old_Long.cu")
file(GLOB SRC_CU1 "TextureTest_Simple.cu")

#create an executable file
#cuda_add_executable(Texture_Test 
#		    ${SRC_H} 
#		    ${SRC_CPP} 
#		    ${SRC_CU})

cuda_add_executable(volumespider
		    ${SRC_H} 
		    ${SRC_CPP} 
		    ${SRC_CU1})



#set the link libraries
#target_link_libraries(Texture_Test
#		  ${OpenCV_LIBS}
#		  ${CMAKE_THREAD_LIBS_INIT}
#		  ${X11_LIBRARIES}
#		  ${Boost_LIBRARIES} 
#		  ${OPENGL_gl_LIBRARY}
#		  ${OPENGL_glu_LIBRARY}	
#		  ${GLEW_LIBRARY}	
#		  ${GLUT_glut_LIBRARY}
#		  ${ANN_LIBRARY}
#		  )

target_link_libraries(volumespider
		  ${OpenCV_LIBS}
		  ${CMAKE_THREAD_LIBS_INIT}
		  ${X11_LIBRARIES}
		  ${Boost_LIBRARIES} 
		  ${OPENGL_gl_LIBRARY}
		  ${OPENGL_glu_LIBRARY}	
		  ${GLEW_LIBRARY}	
		  ${GLUT_glut_LIBRARY}
		  ${ANN_LIBRARY}
		  )

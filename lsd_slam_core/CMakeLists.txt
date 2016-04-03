cmake_minimum_required(VERSION 2.8.7)
project(lsd_slam_core)
# include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
# set(ROS_BUILD_TYPE Release)

# rosbuild_init()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH   ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Eigen3 REQUIRED)
find_package(SuiteParse REQUIRED) # Apparently needed by g2o
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED thread)
# find_package(X11 REQUIRED)

if(${USE_PANGOLIN})
  find_package(Pangolin 0.1 REQUIRED)
endif()

# FabMap
# uncomment this part to enable fabmap
#add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/openFabMap)
#include_directories(${PROJECT_SOURCE_DIR}/thirdparty/openFabMap/include)
#add_definitions("-DHAVE_FABMAP")
#set(FABMAP_LIB openFABMAP )

# SSE/NEON flags
if(${USE_SSE})
  add_definitions("-DENABLE_SSE")
  set(VECTOR_FLAGS "-msse4.1 -msse3 -msse2 -msse")
endif()

if(${USE_NEON})
  add_definitions("-DENABLE_NEON")
  set(VECTOR_FLAGS "-mfpu=neon -funsafe-math-optimizations")
endif()


# Also add some useful compiler flag
set(CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS} ${VECTOR_FLAGS} -march=native -std=c++11"
)

# Set source files
set(lsd_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/DataStructures/Frame.cpp
  ${PROJECT_SOURCE_DIR}/src/DataStructures/FramePoseStruct.cpp
  ${PROJECT_SOURCE_DIR}/src/DataStructures/FrameMemory.cpp
  ${PROJECT_SOURCE_DIR}/src/SlamSystem.cpp
  ${PROJECT_SOURCE_DIR}/src/LiveSLAMWrapper.cpp
  ${PROJECT_SOURCE_DIR}/src/DepthEstimation/DepthMap.cpp
  ${PROJECT_SOURCE_DIR}/src/DepthEstimation/DepthMapPixelHypothesis.cpp
  ${PROJECT_SOURCE_DIR}/src/util/globalFuncs.cpp
  ${PROJECT_SOURCE_DIR}/src/util/log.cpp
  ${PROJECT_SOURCE_DIR}/src/util/SophusUtil.cpp
  ${PROJECT_SOURCE_DIR}/src/util/settings.cpp
  ${PROJECT_SOURCE_DIR}/src/util/Undistorter.cpp
  ${PROJECT_SOURCE_DIR}/src/Tracking/Sim3Tracker.cpp
  ${PROJECT_SOURCE_DIR}/src/Tracking/Relocalizer.cpp
  ${PROJECT_SOURCE_DIR}/src/Tracking/SE3Tracker.cpp
  ${PROJECT_SOURCE_DIR}/src/Tracking/TrackingReference.cpp
  ${PROJECT_SOURCE_DIR}/src/IOWrapper/Timestamp.cpp
  ${PROJECT_SOURCE_DIR}/src/GlobalMapping/FabMap.cpp
  ${PROJECT_SOURCE_DIR}/src/GlobalMapping/KeyFrameGraph.cpp
  ${PROJECT_SOURCE_DIR}/src/GlobalMapping/g2oTypeSim3Sophus.cpp
  ${PROJECT_SOURCE_DIR}/src/GlobalMapping/TrackableKeyFrameSearch.cpp
)

if(${USE_PANGOLIN})
  set(PANGOLIN_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/GUI.cpp
    ${PROJECT_SOURCE_DIR}/src/util/RawLogReader.cpp
    ${PROJECT_SOURCE_DIR}/src/IOWrapper/Pangolin/PangolinOutput3DWrapper.cpp
  )
endif()

set(SOURCE_FILES
  ${lsd_SOURCE_FILES}
  ${PANGOLIN_SOURCE_FILES}
  # ${PROJECT_SOURCE_DIR}/src/IOWrapper/OpenCV/ImageDisplay_OpenCV.cpp
  ${PROJECT_SOURCE_DIR}/src/IOWrapper/Null/ImageDisplay_Null.cpp
)

include_directories(
  ${EIGEN3_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/thirdparty/Sophus
  ${CSPARSE_INCLUDE_DIR} #Has been set by SuiteParse
  ${CHOLMOD_INCLUDE_DIR} #Has been set by SuiteParse
)


message(${OpenCV_LIBS})
# build shared library.
add_library(lsdslam SHARED ${SOURCE_FILES})
target_link_libraries(lsdslam ${FABMAP_LIB}
  g2o_core
  g2o_stuff
  cxsparse
  g2o_solver_csparse
  g2o_csparse_extension
  g2o_types_sim3
  g2o_types_sba
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES}
  ${Pangolin_LIBRARIES}
# X11
)

install(TARGETS lsdslam LIBRARY DESTINATION lib)
install(DIRECTORY src/ DESTINATION include/LsdSlam
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY thirdparty/Sophus/sophus DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

if(${USE_PANGOLIN})
  add_executable(LSD src/LSD.cpp)
  target_link_libraries(LSD lsdslam ${Pangolin_LIBRARIES})
endif()

# vim: sw=2 ts=2 sts=2 et

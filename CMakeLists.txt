cmake_minimum_required(VERSION 3.0.0)

project("SoftRasterization")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(OPENCV_PATH ${PROJECT_SOURCE_DIR}/opencv)

include_directories(${PROJECT_SOURCE_DIR}/glm ./include)

if(${APPLE})
  find_package(PkgConfig REQUIRED)
  pkg_search_module(Opencv REQUIRED opencv4)
  include_directories(${Opencv_INCLUDE_DIRS})
  link_directories(${Opencv_LIBRARY_DIRS})

else()
  include_directories(${OPENCV_PATH}/include ${PROJECT_SOURCE_DIR}/include)
  link_directories(${OPENCV_PATH}/lib)

  file(COPY ${OPENCV_PATH}/bin/opencv_videoio_ffmpeg480_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
  file(COPY ${OPENCV_PATH}/bin/opencv_videoio_msmf480_64.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
  file(COPY ${OPENCV_PATH}/bin/opencv_videoio_msmf480_64d.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
  file(COPY ${OPENCV_PATH}/bin/opencv_world480.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
  file(COPY ${OPENCV_PATH}/bin/opencv_world480d.dll DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

aux_source_directory(${PROJECT_SOURCE_DIR}/source SRC)
add_executable(SoftRasterization main.cpp ${SRC})

message(Libs: ${OPENCV_PATH}/include)
message(Libraries: ${OPENCV_PATH}/lib)

if(${WIN32})
  target_link_libraries(SoftRasterization opencv_world480.lib)
else()
  target_link_libraries(SoftRasterization ${Opencv_LIBRARIES})
endif()

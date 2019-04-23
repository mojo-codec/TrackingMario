cmake_minimum_required(VERSION 2.6)
project(TrackingMario)

SET(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -D_GLIBCXX_USE_CXX11_ABI=0")
SET(TENSORFLOWDIR /opt/tensorflow/1.12.0)
SET(OPENCVDIR /opt/OpenCV_3.4.1)

include_directories(${TENSORFLOWDIR}/include)
include_directories(${TENSORFLOWDIR}/include/external/nsync/public) 
include_directories(${OPENCVDIR}/include)

link_directories(${TENSORFLOWDIR}/lib)
link_directories(${OPENCVDIR}/lib64)


add_executable(TrackingMario main.cpp Tracking.cpp)
target_link_libraries(TrackingMario tensorflow_cc tensorflow_framework)
target_link_libraries(TrackingMario opencv_core opencv_imgproc opencv_highgui opencv_videoio)

#install(TARGETS TrackingMario RUNTIME DESTINATION bin)

project(Img2Gray)

find_package(OpenCV 4.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main img2gray.cc)
target_link_libraries(main ${OpenCV_LIBS})

message(${OpenCV_INCLUDE_DIRS})
message(${OpenCV_LIBS})
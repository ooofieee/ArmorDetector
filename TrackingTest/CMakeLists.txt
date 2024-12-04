# CMakeList.txt: TrackingTest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
include_directories("C:/Users/ofine/opencv/build/include")
link_directories("C:/Users/ofine/opencv/build/x64/vc16/lib")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/resources)

project(TrackingTest)

# 将源代码添加到此项目的可执行文件。
add_executable (TrackingTest "src/TrackingTest.cpp"  "src/findLightBar.cpp" "src/findNum.cpp")

target_link_libraries(TrackingTest opencv_world4100d.lib)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TrackingTest PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

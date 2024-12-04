# 设置包含目录
include_directories("./include")
include_directories("./materials/include")
include_directories("./materials/advanced/include")
include_directories("./mesh/include")
include_directories("./light/include")
include_directories("./renderer/include")
include_directories("./framebuffer/include")
include_directories("./tools/include")
include_directories("../common/include")
include_directories("../application/camera/include")

file(GLOB_RECURSE GLFRAMEWORK ./ *.cpp)

set(ProjectDir ${CMAKE_SOURCE_DIR})
add_definitions(-DPROJECT_DIR="${ProjectDir}")

add_library(glframework ${GLFRAMEWORK}
        ../application/camera/src/orthographicCamera.cpp)

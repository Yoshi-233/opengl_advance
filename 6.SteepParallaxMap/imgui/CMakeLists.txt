# 设置包含目录
file(GLOB_RECURSE IMGUI . *.cpp)

include_directories("./backends")

add_library(imguilib ${IMGUI})

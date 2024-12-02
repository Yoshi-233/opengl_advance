# 设置包含目录
include_directories("./include")
include_directories("../common/include")
include_directories("./app/")
include_directories("./camera/include")
include_directories("./assimpLoader/include")
include_directories("../glframework/include")

add_library(application app/Application.cpp
        ../common/src/check_err.cpp
        camera/src/camera.cpp
        camera/src/orthographicCamera.cpp
        camera/src/perspectiveCamera.cpp
        camera/src/cameraControl.cpp
        camera/src/trackballCameraController.cpp
        camera/src/gameCameraController.cpp
        assimpLoader/src/assimpLoader.cpp
        assimpLoader/src/assimpInstanceLoader.cpp)

set(ProjectDir ${CMAKE_SOURCE_DIR})
add_definitions(-DPROJECT_DIR_INAPP="${ProjectDir}")

# 添加依赖库
target_link_libraries(application "${PROJECT_SOURCE_DIR}/../thirdParty/lib/libglfw3.a")
target_link_libraries(application pthread)

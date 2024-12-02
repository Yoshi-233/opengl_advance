#include "glframework/include/core.h"
#include "glframework/include/shader.hpp"
#include "glframework/include/texture.h"
#include "glframework/include/geometry.h"
#include "application/app/Application.h"
#include "common/include/check_err.h"
#include "common/include/common.h"

#include "application/camera/include/camera.h"
#include "application/camera/include/orthographicCamera.h"
#include "application/camera/include/perspectiveCamera.h"
#include "application/camera/include/trackballCameraController.h"
#include "application/camera/include/gameCameraController.h"

extern std::shared_ptr<CameraControl> cameraControl;

extern void prepareAll();

extern void setAPPFunctions();

extern void doTransform();

extern void render();

extern void initImgui();

extern void renderImgui();

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

int main()
{
        cpp_init();
        if (!APP.init(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL")) {
                ERROR("Failed to initialize Application");
                return -1;
        }

        setAPPFunctions();

        // opengl渲染的区域
        GL_CHECK_ERR(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        // 设置清屏颜色
        GL_CHECK_ERR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

        prepareAll();
        initImgui();

        while (APP.update()) {
                doTransform();
                cameraControl->update();
                render();
                renderImgui();
        }
        APP.close();
        return 0;
}
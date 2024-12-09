//
// Created by nio on 24-9-19.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "../../include/core.h"

enum class MaterialType {
        PhongMaterial,
        PhongEnvMaterial,
        PhongInstancedMaterial,
        GrassInstancedMaterial,
        WhiteMaterial,
        DepthMaterial,
        OpacityMaskMaterial,
        ScreenMaterial,
        CubeMaterial,

        /* advanded */
        PhongNormalMaterial,
        PhongParallaxMaterial,
        PhongShadowMaterial,
        PhongCSMShadowMaterial,

        PhongPointShadowMaterial
};

class Material {
public:
        Material();

        ~Material();

public:
        MaterialType mType;

public:
        /* 深度检测相关 */
        bool mDepthTest{true};
        // 这里如果小于会出现zfight
        // 因为清理深度缓存时会变为1，此时天空盒也是1，导致zfighting
        GLenum mDepthFunc{GL_LEQUAL};
        bool mDepthWrite{true};

public:
        /* PolygonOffset */
        bool mPolygonOffset{false};
        unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
        float mFactor{0.0f};
        float mUnit{0.0f};

public:
        /* 模板测试 */
        bool mStencilTest{false};
        unsigned int mSFail{GL_KEEP}; // 如果模板测试失败
        unsigned int mZFail{GL_KEEP}; // 如果模板测试通过但深度测试失败
        unsigned int mZPass{GL_KEEP}; // 如果模板测试通过且深度测试通过

        unsigned int mStencilMask{0xFF}; // 用于控制模板写入

        unsigned int mStencilFunc{GL_ALWAYS}; // 模板测试函数
        unsigned int mStencilRef{0}; // 模板参考值
        unsigned int mStencilFuncMask{0xFF}; // 模板值掩码
public:
        /* 颜色混合 */
        bool mBlend{false};
        unsigned int mSFactor{GL_SRC_ALPHA};
        unsigned int mDFactor{GL_ONE_MINUS_SRC_ALPHA};

        float mOpacity{1.0f}; // 总体透明度，默认不透明

public:
        /* 面剔除 */
        bool mFaceCulling{false};
        unsigned int mFronFace{GL_CCW}; // 逆时针为正面
        unsigned int mCullFace{GL_BACK}; // 背面剔除

};

#endif //RENDERER_MATERIAL_H

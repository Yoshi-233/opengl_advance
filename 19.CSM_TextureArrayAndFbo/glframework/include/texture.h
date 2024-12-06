//
// Created by Shaozheming on 2024/9/3.
//

#ifndef TEXTURE_TEXTURE_H
#define TEXTURE_TEXTURE_H

#include "check_err.h"
#include "common.h"
#include "core.h"

class Texture {
public:
        static std::shared_ptr<Texture> createTexture(const std::string &path, unsigned int unit);

        static std::shared_ptr<Texture> createTextureFromMemory(const std::string &path, unsigned int unit,
                                                                unsigned char *dataIN, int width, int height);

        static std::shared_ptr<Texture> createColorAttachment(unsigned int unit, int width, int height);

        static std::shared_ptr<Texture> createDepthAttachment(unsigned int unit, int width, int height);

        static std::shared_ptr<Texture> createDepthAttachmentCSMArray(unsigned int unit, int width, int height, unsigned int layerNum);

        static std::shared_ptr<Texture> createDepthStencilAttachment(unsigned int unit, int width, int height);
public:
        Texture(const std::string &path, unsigned int unit, unsigned int internalFormat = GL_RGBA);

        // cube 纹理
        Texture(const std::vector<std::string> &paths, unsigned int unit, unsigned int internalFormat = GL_RGBA);

        // 从内存当中加载纹理，fbx文件自带
        Texture(unsigned int unit, unsigned char *dataIN, int width, int height, unsigned int internalFormat = GL_RGBA);

        // 空白纹理
        Texture(unsigned int unit, int width, int height, unsigned int internalFormat = GL_RGBA);

        Texture();

        ~Texture();

public:
        void bind() const;

        void setUnit(unsigned int unit);

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] GLuint getTextureID() const;

private:
        GLuint mTexture{0};
        int mWidth{0};
        int mHeight{0};
        unsigned int mUnit{0}; // GL_TEXTURE0 + mUnit 表示绑定的纹理单元

        unsigned int mTextureTarget{GL_TEXTURE_2D}; // 纹理数据缓存

        // 纹理缓存
        static std::unordered_map<std::string, std::shared_ptr<Texture>> mTextureCache;
};

#endif //TEXTURE_TEXTURE_H

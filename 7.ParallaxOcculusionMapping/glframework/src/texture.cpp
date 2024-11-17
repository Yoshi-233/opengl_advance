//
// Created by Shaozheming on 2024/9/3.
//
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::mTextureCache{};

std::shared_ptr<Texture> Texture::createTexture(const std::string &path, unsigned int unit)
{
        if (!fileExists(path)) {
                ERROR("Texture is not found, path is {}", path);
        }

        /* 1. 是否缓存过 */
        auto iter = Texture::mTextureCache.find(path);
        if (iter != Texture::mTextureCache.end()) {
                return iter->second;
        }

        /* 2. 创建纹理对象 */
        auto texture = std::make_shared<Texture>(path, unit, GL_SRGB_ALPHA);

        /* 3. 缓存 */
        Texture::mTextureCache[path] = texture;

        return texture;
}

std::shared_ptr<Texture>
Texture::createTextureFromMemory(const std::string &path, unsigned int unit, unsigned char *dataIN, int width,
                                 int height)
{
        if (!fileExists(path)) {
                ERROR("Texture is not found, path is {}", path);
        }

        /* 1. 是否缓存过 */
        auto iter = Texture::mTextureCache.find(path);
        if (iter != Texture::mTextureCache.end()) {
                return iter->second;
        }

        /* 2. 创建纹理对象 */
        auto texture = std::make_shared<Texture>(unit, dataIN, width, height,
                                                 GL_SRGB_ALPHA);

        /* 3. 缓存 */
        Texture::mTextureCache[path] = texture;

        return texture;
}

Texture::Texture(const std::string &path, unsigned int unit, unsigned int internalFormat)
{
        this->mUnit = unit;
        int channels;

        if (!fileExists(path)) {
                ERROR("Texture is not found, path is {}", path);
                return;
        }

        /* 1. 读取图片时要反转y轴 */
        stbi_set_flip_vertically_on_load(true);
        glm::u8 *data = stbi_load(path.c_str(),
                                  &this->mWidth, &this->mHeight, &channels, STBI_rgb_alpha);


        /* 2. 生成1个纹理对象 */
        glGenTextures(1, &this->mTexture);
        /* 激活纹理单元 */
        glActiveTexture(GL_TEXTURE0 + this->mUnit); // 激活0号纹理
        /* 绑定纹理对象，为2D纹理， 如果后续再绑定别的纹理会替换当前纹理 */

        glBindTexture(GL_TEXTURE_2D, this->mTexture);

        /* 3.传输纹理数据到gpu, 该行为会开辟显存 */
        // param0: 2d texture
        // param1: level = 0表示单层纹理，mipmap等级
        // param2: format, gpu上的图片格式
        // param7: 传入图片的数据格式
        // param8: 传入图片的数据格式的存储格式
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->mWidth, this->mHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // TODO:先注掉
        // glGenerateMipmap(GL_TEXTURE_2D);

        /* 4.释放数据， 传输完纹理数据后需要释放data */
        stbi_image_free(data);

        /* 5.设置纹理过滤方式 */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        /* 6.设置纹理wraping方式 */
        // GL_TEXTURE_WRAP_S = u, GL_TEXTURE_WRAP_T = v
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // 注意这里不能使用glBindTexture(GL_TEXTURE_2D, 0);解绑
}

Texture::Texture(unsigned int unit, unsigned char *dataIn, int width, int height, unsigned int internalFormat)
{
        this->mUnit = unit;
        int channels;
        /* 1. 读取图片时要反转y轴 */
        stbi_set_flip_vertically_on_load(true);

        // 计算图片大小， 如果是打包或内嵌纹理，height为0， width为实际图片大小
        glm::u32 size = 0;
        if (height == 0) {
                size = width;
        } else {
                // 统一为RGBA格式
                size = width * height * 4;
        }
        glm::u8 *data = stbi_load_from_memory(dataIn, size,
                                              &this->mWidth, &this->mHeight, &channels, STBI_rgb_alpha);


        /* 2. 生成1个纹理对象 */
        glGenTextures(1, &this->mTexture);
        /* 激活纹理单元 */
        glActiveTexture(GL_TEXTURE0 + this->mUnit); // 激活0号纹理
        /* 绑定纹理对象，为2D纹理， 如果后续再绑定别的纹理会替换当前纹理 */
        glBindTexture(GL_TEXTURE_2D, this->mTexture);

        /* 3.传输纹理数据到gpu, 该行为会开辟显存 */
        // param0: 2d texture
        // param1: level = 0表示单层纹理，mipmap等级
        // param2: format, gpu上的图片格式
        // param7: 传入图片的数据格式
        // param8: 传入图片的数据格式的存储格式
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->mWidth, this->mHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /* 4.释放数据， 传输完纹理数据后需要释放data */
        stbi_image_free(data);

        /* 5.设置纹理过滤方式 */
        // 正常情况下，当需要的像素大于原始image, GL_LINEAR, else GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // 这里如果想用mipmap, 则需要设置GL_TEXTURE_MIN_FILTER为GL_LINEAR_MIPMAP_LINEAR, 否则为GL_NEAREST
        // GL_LINEAR是在单个mipmap层级上进行线性插值，GL_NEAREST是在单个mipmap层级上进行最近邻插值
        // MIPMAP_LINEAR是在所有mipmap层级上进行线性插值
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

        /* 6.设置纹理wraping方式 */
        // GL_TEXTURE_WRAP_S = u, GL_TEXTURE_WRAP_T = v
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(unsigned int unit, int width, int height, unsigned int internalFormat)
{
        this->mUnit = unit;
        this->mWidth = width;
        this->mHeight = height;

        glGenTextures(1, &this->mTexture);
        /* 激活纹理单元 */
        glActiveTexture(GL_TEXTURE0 + this->mUnit); // 激活0号纹理
        /* 绑定纹理对象，为2D纹理， 如果后续再绑定别的纹理会替换当前纹理 */
        glBindTexture(GL_TEXTURE_2D, this->mTexture);

        /* 3.传输纹理数据到gpu, 该行为会开辟显存 */
        // param0: 2d texture
        // param1: level = 0表示单层纹理，mipmap等级
        // param2: format, gpu上的图片格式
        // param7: 传入图片的数据格式
        // param8: 传入图片的数据格式的存储格式
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, this->mWidth, this->mHeight,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        /* 4.设置纹理过滤方式 */
        // 正常情况下，当需要的像素大于原始image, GL_LINEAR, else GL_NEAREST
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

std::shared_ptr<Texture> Texture::createColorAttachment(unsigned int unit, int width, int height)
{
        return std::make_shared<Texture>(unit, width, height);
}

std::shared_ptr<Texture> Texture::createDepthStencilAttachment(unsigned int unit, int width, int height)
{
        std::shared_ptr<Texture> dsTexture = std::make_shared<Texture>();

        unsigned int depthStencil;
        glGenTextures(1, &depthStencil);
        glBindTexture(GL_TEXTURE_2D, depthStencil);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
                     0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        dsTexture->mTexture = depthStencil;
        dsTexture->mUnit = unit;
        dsTexture->mWidth = width;
        dsTexture->mHeight = height;

        return dsTexture;
}

// 右左上下后前(+x -x +y -y +z -z)
Texture::Texture(const std::vector<std::string> &paths, unsigned int unit, unsigned int internalFormat)
{
        this->mUnit = unit;
        this->mTextureTarget = GL_TEXTURE_CUBE_MAP;

        for (auto &path: paths) {
                if (!fileExists(path)) {
                        ERROR("Texture is not found, path is {}", path);
                        return;
                }
        }

        // cubemap不需要反转y轴
        stbi_set_flip_vertically_on_load(false);

        /* 1. 创建cubemap对象 */
        glGenTextures(1, &this->mTexture);
        glActiveTexture(GL_TEXTURE0 + this->mUnit); // 激活0号纹理
        glBindTexture(this->mTextureTarget, this->mTexture);

        /* 2. 循环读取6张图片并且放在cubemap的6个面上 */
        int width = 0, height = 0, channels;
        glm::u8 *data = nullptr;

        for (int i = 0; i < paths.size(); i++) {
                data = stbi_load(paths[i].c_str(),
                                 &width, &height, &channels, STBI_rgb_alpha);

                if (data == nullptr) {
                        ERROR("Failed to load texture: {}", paths[i]);
                        stbi_image_free(data);
                        return;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height,
                             0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);

        }
        /* 3. 设置纹理参数 */
        glTexParameteri(this->mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(this->mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(this->mTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(this->mTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

Texture::Texture()
= default;

Texture::~Texture()
{
        if (this->mTexture != 0) {
                glDeleteTextures(1, &this->mTexture);
        }
}

void Texture::bind() const
{
        /* 激活纹理单元, 这一步必须, 如果其他纹理单元激活会不小心绑定错误 */
        glActiveTexture(GL_TEXTURE0 + this->mUnit);
        /* 绑定纹理对象，为2D纹理， 如果后续再绑定别的纹理会替换当前纹理 */
        glBindTexture(this->mTextureTarget, this->mTexture);
}

int Texture::getWidth() const
{
        return this->mWidth;
}

int Texture::getHeight() const
{
        return this->mHeight;
}

GLuint Texture::getTextureID() const
{
        return this->mTexture;
}


void Texture::setUnit(unsigned int unit)
{
        this->mUnit = unit;
}
















//
// Created by nio on 24-8-21.
//
#include "shader.hpp"
#include <string>
#include <fstream>
#include <sstream>

Shader *shader = nullptr;

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
        // code
        std::string vertexCode;
        std::string fragmentCode;

        try {
                vertexCode = loadShaderFile(vertexPath);
                fragmentCode = loadShaderFile(fragmentPath);
        } catch (std::ifstream::failure &e) {
                ERROR("shader file error: {}.\n", e.what());
        }

        this->setProgram(vertexCode, fragmentCode);
}

Shader::~Shader()
= default;

std::string Shader::loadShaderFile(const std::string &filePath) const
{
        std::ifstream file(filePath);
        std::stringstream shaderStream;
        std::string line;
        while (std::getline(file, line)) {
                // 判断是否include
                if (line.find("#include") != std::string::npos) {
                        // 读取include文件
                        auto start = line.find('\"');
                        auto end = line.find_last_of('\"');
                        std::string includeFile = line.substr(start + 1, end - start - 1);

                        // 找到include文件路径
                        auto lastSlashPos = filePath.find_last_of("/\\");
                        auto folder = filePath.substr(0, lastSlashPos + 1);
                        auto totalPath = folder + includeFile;

                        // 读取include文件内容
                        shaderStream << loadShaderFile(totalPath);
                } else {
                        shaderStream << line << "\n";
                }
        }
        file.close();

        return shaderStream.str();
}

void Shader::begin() const
{
        GL_CHECK_ERR(glUseProgram(this->mProgram));
}

void Shader::end()
{
        GL_CHECK_ERR(glUseProgram(0));
}

void Shader::setProgram(std::string &vertexCode, std::string &fragmentCode)
{
        const char *vertexShaderSource = vertexCode.c_str();
        const char *fragmentShaderSource = fragmentCode.c_str();
        /* create shader */
        GLuint vertexShader, fragmentShader;
        vertexShader = GL_CHECK_ERR(glCreateShader(GL_VERTEX_SHADER));
        fragmentShader = GL_CHECK_ERR(glCreateShader(GL_FRAGMENT_SHADER));

        /* input shader code */
        GL_CHECK_ERR(glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr));
        GL_CHECK_ERR(glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr));

        /* compile */
        glCompileShader(vertexShader);
        checkShaderErrors(vertexShader, "COMPILE");
        glCompileShader(fragmentShader);
        checkShaderErrors(fragmentShader, "COMPILE");

        /* link */
        this->mProgram = glCreateProgram();
        glAttachShader(this->mProgram, vertexShader);
        glAttachShader(this->mProgram, fragmentShader);
        glLinkProgram(this->mProgram);
        checkShaderErrors(this->mProgram, "LINK");

        /* clear code */
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
}

void Shader::checkShaderErrors(GLuint target, std::string &&type) const
{
        int success = 0;
        char infoLog[1024];
        if (type == "COMPILE") {
                glGetShaderiv(target, GL_COMPILE_STATUS, &success);
                if (!success) {
                        glGetShaderInfoLog(target, 1024, nullptr, infoLog);
                        ERROR(infoLog);
                }
        } else if (type == "LINK") {
                glGetProgramiv(this->mProgram, GL_LINK_STATUS, &success);
                if (!success) {
                        glGetProgramInfoLog(this->mProgram, 1024, nullptr, infoLog);
                        ERROR(infoLog);
                }
        } else {
                ERROR("type({}) error!", type.c_str());
        }
}

void Shader::setFloat(const std::string &name, float &&value) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform1f(location, value));
}

void Shader::setFloat(const std::string &name, const float &value) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform1f(location, value));
}

void Shader::setVectorFloat(const std::string &name, float *value, int count) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform1fv(location, count, value));
}


void Shader::setVectorFloat(const std::string &name, const GLfloat *array) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform3fv(location, 1, (const GLfloat *) array));
}

void Shader::setVectorFloat(const std::string &name, float &&x, float &&y, float &&z) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform3f(location, x, y, z));
}

void Shader::setVectorFloat(const std::string &name, const glm::vec3 &array) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform3f(location, array.x, array.y, array.z));
}

void Shader::setInt(const std::string &name, int &value) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform1i(location, value));
}

void Shader::setInt(const std::string &name, int &&value) const
{
        GLint location = GL_CHECK_ERR(glGetUniformLocation(this->mProgram, name.c_str()));

        GL_CHECK_ERR(glUniform1i(location, value));
}








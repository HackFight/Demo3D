#include "RendererAPI/ShaderManager.h"

// libs
#include <glm/gtc/type_ptr.hpp>

// std
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Core
{
    std::vector<GLuint> ShaderManager::shaders;
    uint32_t ShaderManager::CreateShader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // shader Program
        GLuint shader = glCreateProgram();
        glAttachShader(shader, vertex);
        glAttachShader(shader, fragment);
        glLinkProgram(shader);
        // print linking errors if any
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

            glDeleteProgram(shader);
            shader = 0;
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        shaders.push_back(shader);
        return shaders.size() - 1;
    }

    void ShaderManager::Bind(uint32_t shader)
    {
        glUseProgram(shaders[shader]);
    }
    void ShaderManager::Unbind()
    {
        glUseProgram(0);
    }

    void ShaderManager::setBool(uint32_t shader, const std::string& name, bool value)
    {
        Bind(shader);
        glUniform1i(glGetUniformLocation(shaders[shader], name.c_str()), (int)value);
    }
    void ShaderManager::setInt(uint32_t shader, const std::string& name, int value)
    {
        Bind(shader);
        glUniform1i(glGetUniformLocation(shaders[shader], name.c_str()), value);
    }
    void ShaderManager::setFloat(uint32_t shader, const std::string& name, float value)
    {
        Bind(shader);
        glUniform1f(glGetUniformLocation(shaders[shader], name.c_str()), value);
    }
    void ShaderManager::set2f(uint32_t shader, const std::string& name, float v1, float v2)
    {
        Bind(shader);
        glUniform2f(glGetUniformLocation(shaders[shader], name.c_str()), v1, v2);
    }
    void ShaderManager::set2f(uint32_t shader, const std::string& name, glm::vec2 v)
    {
        Bind(shader);
        glUniform2f(glGetUniformLocation(shaders[shader], name.c_str()), v.x, v.y);
    }
    void ShaderManager::set3f(uint32_t shader, const std::string& name, float v1, float v2, float v3)
    {
        Bind(shader);
        glUniform3f(glGetUniformLocation(shaders[shader], name.c_str()), v1, v2, v3);
    }
    void ShaderManager::set3f(uint32_t shader, const std::string& name, glm::vec3 v)
    {
        Bind(shader);
        glUniform3f(glGetUniformLocation(shaders[shader], name.c_str()), v.x, v.y, v.z);
    }

    void ShaderManager::setmat4(uint32_t shader, const std::string& name, glm::mat4 mat)
    {
        Bind(shader);
        glUniformMatrix4fv(glGetUniformLocation(shaders[shader], name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void ShaderManager::ReleaseAll()
    {
        for(GLuint shader : shaders)
        {
            glDeleteProgram(shader);
            shader = 0;
        }
    }
}
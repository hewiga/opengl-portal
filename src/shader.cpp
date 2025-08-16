#include "shader.h"

Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath) {

    std::string vertexShaderSource = readShaderData(vertexShaderPath);
    std::string fragmentShaderSource = readShaderData(fragmentShaderPath);

    const char* vertexShaderData = vertexShaderSource.c_str();
    const char* fragmentShaderData = fragmentShaderSource.c_str();


    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderData, NULL);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderData, NULL);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);

    glLinkProgram(m_id);
    compileErrors(m_id, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string Shader::readShaderData(std::string shaderPath) {

    std::fstream shaderFile("res/shaders/" + shaderPath);
    if (!shaderFile.is_open()) {
        std::cout << "Couldn't open file " << shaderPath << std::endl;
        return "";
    }

    shaderFile.seekg(0, std::ios::end);
    unsigned int fileLength = shaderFile.tellg();
    shaderFile.seekg(0);

    std::string shaderData(fileLength, ' ');
    shaderFile.read(&shaderData[0], fileLength);

    shaderFile.close();

    return shaderData;
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
    // Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
        }
    }
}

void Shader::setMat4(std::string name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::activateShader() {

    glUseProgram(m_id);
}

void Shader::deleteShader() {

    glDeleteProgram(m_id);
}
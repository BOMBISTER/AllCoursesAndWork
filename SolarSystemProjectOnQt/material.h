#pragma once

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <memory>
#include <string>

class Material {
public:
    Material(const std::string& dayTexturePath, const std::string& normalMapPath);
    void initialize(QOpenGLFunctions* glFunctions);
    void bind(QOpenGLShaderProgram* program, bool useTexture, bool useNormalMap, bool useNightTexture);

private:
    std::shared_ptr<QOpenGLTexture> m_dayTexture;
    std::shared_ptr<QOpenGLTexture> m_normalMap;
};

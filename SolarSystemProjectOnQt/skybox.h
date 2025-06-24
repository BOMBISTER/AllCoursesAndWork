#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

class Skybox {
public:
    Skybox();
    void initialize(QOpenGLFunctions *glFunctions);
    void render(QOpenGLFunctions *glFunctions, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix);

private:
    QOpenGLBuffer m_vertexBuffer;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    QOpenGLFunctions *m_glFunctions;
};

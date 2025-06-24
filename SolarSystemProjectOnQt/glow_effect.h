#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>

class GlowEffect {
public:
    GlowEffect();
    void initialize(QOpenGLFunctions *glFunctions);
    void render(QOpenGLFunctions *glFunctions, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, const QVector3D &position, float size);

private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    QOpenGLFunctions *m_glFunctions;
};

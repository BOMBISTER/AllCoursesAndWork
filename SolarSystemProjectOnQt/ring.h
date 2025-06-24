#ifndef RING_H
#define RING_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QVector3D>
#include <QMatrix4x4>

class Ring : protected QOpenGLFunctions {
public:
    Ring();
    ~Ring();

    void initialize();
    void render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, const QVector3D &planetPosition, const QVector3D &rotationAxis);

private:
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;
    std::unique_ptr<QOpenGLTexture> m_texture;
    GLint m_posAttr;
    GLint m_texAttr;
    GLint m_modelMatrixUniform;
    GLint m_viewMatrixUniform;
    GLint m_projectionMatrixUniform;
    GLint m_textureUniform;
};

#endif

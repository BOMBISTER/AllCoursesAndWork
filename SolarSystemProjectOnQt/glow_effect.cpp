#include "glow_effect.h"
#include <QDebug>

GlowEffect::GlowEffect() : m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_indexBuffer(QOpenGLBuffer::IndexBuffer), m_glFunctions(nullptr) {
}

void GlowEffect::initialize(QOpenGLFunctions *glFunctions) {
    m_glFunctions = glFunctions;

    m_program = std::make_unique<QOpenGLShaderProgram>();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glow_vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glow_fragment.fsh");
    if (!m_program->link()) {
        qDebug() << "Glow shader program failed to link:" << m_program->log();
    }


    struct Vertex {
        QVector3D position;
        QVector2D texCoord;
    };
    Vertex vertices[] = {
        {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
    };
    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertices, 4 * sizeof(Vertex));

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices, 6 * sizeof(GLuint));
}

void GlowEffect::render(QOpenGLFunctions *glFunctions, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, const QVector3D &position, float size) {
    m_program->bind();


    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.scale(size);

    QMatrix4x4 viewNoRotation = viewMatrix;
    viewNoRotation.setColumn(0, QVector4D(1.0f, 0.0f, 0.0f, 0.0f));
    viewNoRotation.setColumn(1, QVector4D(0.0f, 1.0f, 0.0f, 0.0f));
    viewNoRotation.setColumn(2, QVector4D(0.0f, 0.0f, 1.0f, 0.0f));

    m_program->setUniformValue("modelMatrix", modelMatrix);
    m_program->setUniformValue("viewMatrix", viewNoRotation);
    m_program->setUniformValue("projectionMatrix", projectionMatrix);

    m_vertexBuffer.bind();
    int posAttr = m_program->attributeLocation("position");
    int texAttr = m_program->attributeLocation("texCoord");
    m_program->enableAttributeArray(posAttr);
    m_program->setAttributeBuffer(posAttr, GL_FLOAT, 0, 3, sizeof(QVector3D) + sizeof(QVector2D));
    m_program->enableAttributeArray(texAttr);
    m_program->setAttributeBuffer(texAttr, GL_FLOAT, sizeof(QVector3D), 2, sizeof(QVector3D) + sizeof(QVector2D));

    m_indexBuffer.bind();
    m_glFunctions->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    m_program->disableAttributeArray(posAttr);
    m_program->disableAttributeArray(texAttr);
    m_program->release();
}

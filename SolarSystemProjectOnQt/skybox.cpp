#include "skybox.h"
#include <QDebug>

Skybox::Skybox() : m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_glFunctions(nullptr) {
}

void Skybox::initialize(QOpenGLFunctions *glFunctions) {
    m_glFunctions = glFunctions;

    m_program = std::make_unique<QOpenGLShaderProgram>();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox_vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox_fragment.fsh");
    if (!m_program->link()) {
        qDebug() << "Skybox shader program failed to link:" << m_program->log();
    }

    float vertices[] = {
        // Back face
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        // Front face
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        // Left face
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        // Right face
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        // Bottom face
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        // Top face
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f
    };

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertices, sizeof(vertices));
}

void Skybox::render(QOpenGLFunctions *glFunctions, const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix) {
    m_program->bind();

    QMatrix4x4 viewNoTranslation = viewMatrix;
    viewNoTranslation.setColumn(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));

    m_program->setUniformValue("viewMatrix", viewNoTranslation);
    m_program->setUniformValue("projectionMatrix", projectionMatrix);

    m_vertexBuffer.bind();
    int posAttr = m_program->attributeLocation("position");
    m_program->enableAttributeArray(posAttr);
    m_program->setAttributeBuffer(posAttr, GL_FLOAT, 0, 3, 3 * sizeof(float));

    m_glFunctions->glDrawArrays(GL_TRIANGLES, 0, 36);

    m_program->disableAttributeArray(posAttr);
    m_program->release();
}

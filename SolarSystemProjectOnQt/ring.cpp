#include "ring.h"
#include <QDebug>
#include <QMatrix4x4>

Ring::Ring() : m_vbo(QOpenGLBuffer::VertexBuffer) {}

Ring::~Ring() {}

void Ring::initialize() {
    initializeOpenGLFunctions();

    m_program = std::make_unique<QOpenGLShaderProgram>();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ring_vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ring_fragment.fsh");
    if (!m_program->link()) {
        qDebug() << "Ring shader program failed to link:" << m_program->log();
    }

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();

    const int segments = 64;
    const float innerRadius = 4.5f;
    const float outerRadius = 8.0f;
    std::vector<GLfloat> vertices;
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float cosA = cos(angle);
        float sinA = sin(angle);

        vertices.push_back(innerRadius * cosA);
        vertices.push_back(innerRadius * sinA);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        vertices.push_back(outerRadius * cosA);
        vertices.push_back(outerRadius * sinA);
        vertices.push_back(0.0f);
        vertices.push_back(1.0f);
        vertices.push_back(0.0f);
    }

    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(GLfloat));

    m_posAttr = m_program->attributeLocation("position");
    m_texAttr = m_program->attributeLocation("texCoord");
    m_modelMatrixUniform = m_program->uniformLocation("modelMatrix");
    m_viewMatrixUniform = m_program->uniformLocation("viewMatrix");
    m_projectionMatrixUniform = m_program->uniformLocation("projectionMatrix");
    m_textureUniform = m_program->uniformLocation("textureSampler");

    glEnableVertexAttribArray(m_posAttr);
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(m_texAttr);
    glVertexAttribPointer(m_texAttr, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    m_vbo.release();
    m_vao.release();

    m_texture = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target2D);
    if (!m_texture->create()) {
        qDebug() << "Failed to create ring texture";
    }
    m_texture->setData(QImage(":/saturn_rings.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);

    qDebug() << "Ring initialized";
}

void Ring::render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix, const QVector3D &planetPosition, const QVector3D &rotationAxis) {
    m_program->bind();
    m_vao.bind();

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(planetPosition);
    QVector3D up = rotationAxis.normalized();
    QVector3D right = QVector3D::crossProduct(up, QVector3D(0, 0, 1)).normalized();
    QVector3D forward = QVector3D::crossProduct(right, up).normalized();
    QMatrix4x4 rotation;
    rotation.setColumn(0, QVector4D(right, 0));
    rotation.setColumn(1, QVector4D(up, 0));
    rotation.setColumn(2, QVector4D(forward, 0));
    rotation.setColumn(3, QVector4D(0, 0, 0, 1));
    modelMatrix *= rotation;

    m_program->setUniformValue(m_modelMatrixUniform, modelMatrix);
    m_program->setUniformValue(m_viewMatrixUniform, viewMatrix);
    m_program->setUniformValue(m_projectionMatrixUniform, projectionMatrix);
    m_program->setUniformValue(m_textureUniform, 0);

    m_texture->bind(0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2 * (64 + 1));

    m_texture->release();
    m_vao.release();
    m_program->release();
}

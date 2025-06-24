#include "planet.h"
#include <cmath>
#include <QDebug>

Planet::Planet(float radius, QVector3D initialPos, QVector3D rotationAxis, float orbitRadius, float orbitPeriod, const std::string& dayTexturePath, const std::string& normalMapPath, const std::string& name)
    : m_radius(radius), m_position(initialPos), m_rotationAxis(rotationAxis.normalized()), m_rotationAngle(0.0f),
    m_orbitRadius(orbitRadius), m_orbitPeriod(orbitPeriod), m_orbitAngle(0.0f), m_name(name),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer), m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_material(std::make_unique<Material>(dayTexturePath, normalMapPath)), m_glFunctions(nullptr) {
    qDebug() << "Creating planet:" << QString::fromStdString(name);
}

void Planet::initialize(QOpenGLFunctions* glFunctions) {
    m_glFunctions = glFunctions;
    initSphere();
    m_material->initialize(glFunctions);
    qDebug() << "Initialized planet:" << QString::fromStdString(m_name);
}

void Planet::initSphere() {
    const int lats = 50, longs = 50;
    const float latStep = 2 * M_PI / lats;
    const float longStep = M_PI / longs;

    m_vertices.clear();
    m_indices.clear();

    for (unsigned i = 0; i <= longs; ++i) {
        float phi = i * longStep;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);

        for (unsigned j = 0; j <= lats; ++j) {
            float theta = j * latStep;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            float x = m_radius * sinPhi * cosTheta;
            float z = m_radius * sinPhi * sinTheta;
            float y = m_radius * cosPhi;

            QVector3D pos{x, y, z};
            QVector3D norm = pos.normalized();
            float u = (float)j / lats;
            float v = (float)i / longs;
            QVector3D tangent = QVector3D(-m_radius * sinPhi * sinTheta, 0.0f, m_radius * sinPhi * cosTheta).normalized();
            QVector3D bitangent = QVector3D::crossProduct(norm, tangent).normalized();

            m_vertices.emplace_back(pos, norm, QVector2D(u, v), tangent, bitangent);
        }
    }

    for (unsigned i = 0; i < lats; ++i) {
        unsigned k1 = i * (longs + 1);
        unsigned k2 = k1 + longs + 1;

        for (unsigned j = 0; j < longs; ++j, ++k1, ++k2) {
            if (i != 0) {
                m_indices.push_back(k1);
                m_indices.push_back(k2);
                m_indices.push_back(k1 + 1);
            }
            if (i != lats - 1) {
                m_indices.push_back(k1 + 1);
                m_indices.push_back(k2);
                m_indices.push_back(k2 + 1);
            }
        }
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices.data(), m_vertices.size() * sizeof(VertexData));

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(m_indices.data(), m_indices.size() * sizeof(GLuint));
}

void Planet::render(QOpenGLShaderProgram* program, GLint posAttr, GLint normAttr, GLint textureAttr, GLint tangentAttr, GLint bitangentAttr, bool useTexture, bool useNormalMap, bool useNightTexture) {
    if (!m_glFunctions) {
        qDebug() << "Error: m_glFunctions is null for planet" << QString::fromStdString(m_name);
        return;
    }

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    if (m_name != "Sun") {
        modelMatrix.translate(m_position);
    }
    modelMatrix.rotate(m_rotationAngle, m_rotationAxis);
    program->setUniformValue("model", modelMatrix);
    program->setUniformValue("normMatrix", modelMatrix.normalMatrix());

    m_material->bind(program, useTexture, useNormalMap, useNightTexture);

    m_vertexBuffer.bind();
    m_indexBuffer.bind();

    float offset = 0;
    if (posAttr != -1) {
        program->enableAttributeArray(posAttr);
        program->setAttributeBuffer(posAttr, GL_FLOAT, offset, 3, sizeof(VertexData));
    }
    offset += sizeof(QVector3D);
    if (normAttr != -1) {
        program->enableAttributeArray(normAttr);
        program->setAttributeBuffer(normAttr, GL_FLOAT, offset, 3, sizeof(VertexData));
    }
    offset += sizeof(QVector3D);
    if (textureAttr != -1) {
        program->enableAttributeArray(textureAttr);
        program->setAttributeBuffer(textureAttr, GL_FLOAT, offset, 2, sizeof(VertexData));
    }
    offset += sizeof(QVector2D);
    if (tangentAttr != -1) {
        program->enableAttributeArray(tangentAttr);
        program->setAttributeBuffer(tangentAttr, GL_FLOAT, offset, 3, sizeof(VertexData));
    }
    offset += sizeof(QVector3D);
    if (bitangentAttr != -1) {
        program->enableAttributeArray(bitangentAttr);
        program->setAttributeBuffer(bitangentAttr, GL_FLOAT, offset, 3, sizeof(VertexData));
    }
    m_glFunctions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size() / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);

    GLenum err;
    while ((err = m_glFunctions->glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error in planet" << QString::fromStdString(m_name) << ":" << err;
    }
}

void Planet::updateOrbit(float deltaTime) {
    if (m_orbitPeriod > 0) {
        m_orbitAngle += (2 * M_PI * deltaTime) / m_orbitPeriod;
        m_orbitAngle = fmod(m_orbitAngle, 2 * M_PI);
        m_position = QVector3D(m_orbitRadius * cos(m_orbitAngle), 0.0f, m_orbitRadius * sin(m_orbitAngle));
    }
}

void Planet::rotate(float angle) {
    m_rotationAngle += angle;
    m_rotationAngle = fmod(m_rotationAngle, 360.0f);
}

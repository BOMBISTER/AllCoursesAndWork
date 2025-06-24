#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector2D>
#include <QVector3D>
#include <string>
#include <memory>
#include "material.h"

struct VertexData {
    VertexData(QVector3D p, QVector3D n, QVector2D t, QVector3D tng, QVector3D btg)
        : position(p), normal(n), textcoord(t), tangent(tng), bitangent(btg) {}
    QVector3D position;
    QVector3D normal;
    QVector2D textcoord;
    QVector3D tangent;
    QVector3D bitangent;
};

class Planet {
public:
    Planet(float radius, QVector3D initialPos, QVector3D rotationAxis, float orbitRadius, float orbitPeriod, const std::string& dayTexturePath, const std::string& normalMapPath, const std::string& name);
    void initialize(QOpenGLFunctions* glFunctions);
    void render(QOpenGLShaderProgram* program, GLint posAttr, GLint normAttr, GLint textureAttr, GLint tangentAttr, GLint bitangentAttr, bool useTexture, bool useNormalMap, bool useNightTexture);
    void updateOrbit(float deltaTime);
    void rotate(float angle);
    std::string name() const { return m_name; }
    QVector3D position() const { return m_position; }
    float orbitAngle() const { return m_orbitAngle; }
    float orbitRadius() const { return m_orbitRadius; }
    QVector3D m_position;

private:
    void initSphere();

    float m_radius;

    QVector3D m_rotationAxis;
    float m_rotationAngle;
    float m_orbitRadius;
    float m_orbitPeriod;
    float m_orbitAngle;
    std::string m_name;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    std::unique_ptr<Material> m_material;
    std::vector<VertexData> m_vertices;
    std::vector<GLuint> m_indices;
    QOpenGLFunctions* m_glFunctions;
};

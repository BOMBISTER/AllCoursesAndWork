#pragma once

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>
#include <QOpenGLShaderProgram>
#include "transformational.h"

class Camera3D : public Transformational
{
public:
    Camera3D();

    void draw(QOpenGLShaderProgram* program, QOpenGLFunctions* functions) override;
    void rotate(const QQuaternion& r) override;
    void translate(const QVector3D& t) override;
    void scale(const float& s) override;
    void setGlobalTransform(const QMatrix4x4& g) override;

    QMatrix4x4 viewMatrix() const { return m_viewMatrix; }
    QVector3D position() const { return m_translate; }

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;
    QMatrix4x4 m_viewMatrix;
};

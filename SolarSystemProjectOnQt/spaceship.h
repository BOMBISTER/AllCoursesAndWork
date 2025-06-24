#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <QObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QImage>
#include <QOpenGLFunctions>
#include <QString>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

class SpaceShip : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SpaceShip(QObject *parent = nullptr);
    ~SpaceShip();

    void initializeGL();
    void render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix);
    void update(float deltaTime);

    void setMovingForward(bool moving) { m_movingForward = moving; }
    void setMovingBackward(bool moving) { m_movingBackward = moving; }
    void setMovingLeft(bool moving) { m_movingLeft = moving; }
    void setMovingRight(bool moving) { m_movingRight = moving; }
    void setMovingUp(bool moving) { m_movingUp = moving; }
    void setMovingDown(bool moving) { m_movingDown = moving; }

    QVector3D position() const { return m_position; }
    void setPosition(const QVector3D &position) { m_position = position; }

private:
    bool loadModel(const QString &filename);
    bool loadTexture(const QString &filename);
    void setupVertexAttribs();

    QOpenGLShaderProgram *m_program;
    QOpenGLTexture *m_texture;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ibo;
    QOpenGLVertexArrayObject m_vao;
    QVector<Vertex> m_vertices;
    QVector<unsigned int> m_indices;

    QVector3D m_position;
    float m_rotationAngle;
    float m_speed;
    float m_tiltAngleX;
    float m_tiltAngleZ;
    float m_maxTiltAngle;
    float m_tiltSpeed;
    bool m_movingForward;
    bool m_movingBackward;
    bool m_movingLeft;
    bool m_movingRight;
    bool m_movingUp;
    bool m_movingDown;
};

#endif

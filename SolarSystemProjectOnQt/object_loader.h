#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <vector>
#include <QOpenGLTexture>
#include <QMap>
#include <QList>

struct ObjectMaterial {
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;
    QString diffuseMap;
    QOpenGLTexture* diffuseTexture;

    ObjectMaterial() : ambient(0.2f, 0.2f, 0.2f),
                      diffuse(0.8f, 0.8f, 0.8f),
                      specular(1.0f, 1.0f, 1.0f),
                      shininess(32.0f),
                      diffuseTexture(nullptr) {}
};

struct Face {
    QList<int> indices;
    QString material;

    Face() {}
    Face(const QList<int>& idx, const QString& mat) : indices(idx), material(mat) {}
};

class Object : public QOpenGLFunctions {
public:
    Object(const QString& name = "", const QString& modelPath = "", float scale = 1.0f);
    ~Object();

    bool loadFromFile(const QString& filename);
    void initialize(QOpenGLFunctions* functions);
    void render(QOpenGLShaderProgram* program);

    QVector3D getMaterialColor() const {
        if (!materials.empty()) {
            return materials[0].diffuse;
        }
        return QVector3D(1.0f, 1.0f, 1.0f);
    }

    QString name() const { return m_name; }

private:
    struct Vertex {
        QVector3D position;
        QVector3D normal;
        QVector2D texCoord;
        QVector3D tangent;
        QVector3D bitangent;
    };

    QString m_name;
    QString m_modelPath;
    float m_scale;
    QOpenGLFunctions* glFunctions;

    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    QVector<Vertex> vertices;
    QVector<GLuint> indices;
    QMap<QString, ObjectMaterial> materials;

    void loadTextures();
};

#endif

#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QDebug>

#include "object_loader.h"

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
    QVector3D tangent;
    QVector3D bitangent;
};

Object::Object(const QString& name, const QString& modelPath, float scale)
    : m_name(name)
    , m_modelPath(modelPath)
    , m_scale(scale)
    , glFunctions(nullptr)
{
    vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    ibo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if (!modelPath.isEmpty()) {
        if (loadFromFile(modelPath)) {
            loadTextures();
        }
    }
}

Object::~Object() {
    for (auto it = materials.begin(); it != materials.end(); ++it) {
        if (it.value().diffuseTexture) {
            if (it.value().diffuseTexture->isCreated()) {
                it.value().diffuseTexture->destroy();
            }
            delete it.value().diffuseTexture;
            it.value().diffuseTexture = nullptr;
        }
    }
    
    if (vbo.isCreated()) {
        vbo.bind();
        vbo.destroy();
        vbo.release();
    }
    if (ibo.isCreated()) {
        ibo.bind();
        ibo.destroy();
        ibo.release();
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error in destructor:" << err;
    }
}

bool Object::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filename;
        return false;
    }

    QString mtlFilename = filename;
    mtlFilename.replace(".obj", ".mtl");
    QFile mtlFile(mtlFilename);
    if (mtlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream mtlIn(&mtlFile);
        ObjectMaterial currentMaterial;
        QString currentMaterialName;

        while (!mtlIn.atEnd()) {
            QString line = mtlIn.readLine().trimmed();
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            
            if (parts.isEmpty()) continue;

            if (parts[0] == "newmtl") {
                if (!currentMaterialName.isEmpty()) {
                    materials[currentMaterialName] = currentMaterial;
                }
                currentMaterialName = parts[1];
                currentMaterial = ObjectMaterial();
            }
            else if (parts[0] == "Ka") {
                currentMaterial.ambient = QVector3D(
                    parts[1].toFloat(),
                    parts[2].toFloat(),
                    parts[3].toFloat()
                );
            }
            else if (parts[0] == "Kd") {
                currentMaterial.diffuse = QVector3D(
                    parts[1].toFloat(),
                    parts[2].toFloat(),
                    parts[3].toFloat()
                );
            }
            else if (parts[0] == "Ks") {
                currentMaterial.specular = QVector3D(
                    parts[1].toFloat(),
                    parts[2].toFloat(),
                    parts[3].toFloat()
                );
            }
            else if (parts[0] == "Ns") {
                currentMaterial.shininess = parts[1].toFloat();
            }
            else if (parts[0] == "map_Kd") {
                QString texturePath = parts[1];
                currentMaterial.diffuseMap = texturePath;
            }
        }
        
        if (!currentMaterialName.isEmpty()) {
            materials[currentMaterialName] = currentMaterial;
        }
        
        mtlFile.close();
    }

    QTextStream in(&file);
    QVector<QVector3D> positions;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;
    QVector<Face> faces;
    QString currentMaterial;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);
        
        if (parts.isEmpty()) continue;

        if (parts[0] == "v") {
            // Vertex position
            if (parts.size() >= 4) {
                positions.append(QVector3D(
                    parts[1].toFloat(),
                    parts[2].toFloat(),
                    parts[3].toFloat()
                ));
            }
        }

        else if (parts[0] == "vt") {
            // Texture coordinate
            if (parts.size() >= 3) {
                texCoords.append(QVector2D(
                    parts[1].toFloat(),
                    parts[2].toFloat()
                ));
            }
        }
        else if (parts[0] == "usemtl") {
            currentMaterial = parts[1];
        }
        else if (parts[0] == "f") {
            // Face
            QList<int> faceIndices;
            for (int i = 1; i < parts.size(); ++i) {
                QStringList indices = parts[i].split("/");
                if (indices.size() >= 1) {
                    int posIndex = indices[0].toInt() - 1;
                    int texIndex = indices.size() > 1 && !indices[1].isEmpty() ? indices[1].toInt() - 1 : -1;
                    int normIndex = indices.size() > 2 && !indices[2].isEmpty() ? indices[2].toInt() - 1 : -1;
                    
                    faceIndices.append(posIndex);
                    faceIndices.append(texIndex);
                    faceIndices.append(normIndex);
                }
            }
            faces.append(Face(faceIndices, currentMaterial));
        }
    }

    qDebug() << "Loaded from file:" << filename;
    qDebug() << "  Positions:" << positions.size();
    qDebug() << "  Normals:" << normals.size();
    qDebug() << "  TexCoords:" << texCoords.size();
    qDebug() << "  Faces:" << faces.size();

    vertices.clear();
    indices.clear();
    QMap<QString, int> vertexMap;

    if (normals.isEmpty()) {
        qDebug() << "Computing normals...";
        normals.resize(positions.size(), QVector3D(0, 0, 0));
        
        for (const auto& face : faces) {
            if (face.indices.size() < 9) continue;

            QVector3D v1 = positions[face.indices[0]];
            QVector3D v2 = positions[face.indices[3]];
            QVector3D v3 = positions[face.indices[6]];

            QVector3D normal = QVector3D::crossProduct(v2 - v1, v3 - v1).normalized();


            normals[face.indices[0]] += normal;
            normals[face.indices[3]] += normal;
            normals[face.indices[6]] += normal;
        }

        for (auto& normal : normals) {
            normal.normalize();
        }
    }

    for (const auto& face : faces) {
        if (face.indices.size() < 9) continue;

        for (int i = 0; i < 3; ++i) {
            int posIndex = face.indices[i * 3];
            int texIndex = face.indices[i * 3 + 1];
            int normIndex = face.indices[i * 3 + 2];

            QString vertexKey = QString("%1/%2/%3")
                .arg(posIndex)
                .arg(texIndex)
                .arg(normIndex);

            if (!vertexMap.contains(vertexKey)) {
                Vertex vertex;
                vertex.position = positions[posIndex] * m_scale;
                

                vertex.normal = normIndex >= 0 ? normals[normIndex] : normals[posIndex];
                
                vertex.texCoord = texIndex >= 0 ? texCoords[texIndex] : QVector2D(0, 0);

                if (i == 0) {
                    QVector3D v1 = positions[face.indices[3]] - positions[face.indices[0]];
                    QVector3D v2 = positions[face.indices[6]] - positions[face.indices[0]];
                    QVector2D uv1 = texIndex >= 0 ? texCoords[face.indices[4]] - texCoords[face.indices[1]] : QVector2D(0, 0);
                    QVector2D uv2 = texIndex >= 0 ? texCoords[face.indices[7]] - texCoords[face.indices[1]] : QVector2D(0, 0);

                    float r = 1.0f / (uv1.x() * uv2.y() - uv1.y() * uv2.x());
                    vertex.tangent = (v1 * uv2.y() - v2 * uv1.y()) * r;
                    vertex.bitangent = (v2 * uv1.x() - v1 * uv2.x()) * r;
                } else {
                    vertex.tangent = vertices[vertexMap[QString("%1/%2/%3")
                        .arg(face.indices[0])
                        .arg(face.indices[1])
                        .arg(face.indices[2])]].tangent;
                    vertex.bitangent = vertices[vertexMap[QString("%1/%2/%3")
                        .arg(face.indices[0])
                        .arg(face.indices[1])
                        .arg(face.indices[2])]].bitangent;
                }

                vertices.append(vertex);
                vertexMap[vertexKey] = vertices.size() - 1;
            }
            
            indices.push_back(vertexMap[vertexKey]);
        }
    }

    qDebug() << "Created" << vertices.size() << "vertices and" << indices.size() << "indices";

    return true;
}

void Object::initialize(QOpenGLFunctions* functions) {
    glFunctions = functions;
    initializeOpenGLFunctions();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.constData(), vertices.size() * sizeof(Vertex));
    vbo.release();

    ibo.create();
    ibo.bind();
    ibo.allocate(indices.constData(), indices.size() * sizeof(GLuint));
    ibo.release();
}

void Object::loadTextures() {
    qDebug() << "Loading textures...";
    for (auto it = materials.begin(); it != materials.end(); ++it) {
        if (!it.value().diffuseMap.isEmpty()) {
            QString texturePath = ":/Maps/" + it.value().diffuseMap.remove(0, 1);
            qDebug() << "Loading texture:" << texturePath;
            qDebug() << "Original diffuse map path:" << it.value().diffuseMap;
            
            QImage image(texturePath);
            if (!image.isNull()) {
                qDebug() << "Image loaded successfully, size:" << image.size();
                it.value().diffuseTexture = new QOpenGLTexture(image.mirrored());
                it.value().diffuseTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
                it.value().diffuseTexture->setMagnificationFilter(QOpenGLTexture::Linear);
                it.value().diffuseTexture->setWrapMode(QOpenGLTexture::Repeat);
                qDebug() << "Texture loaded successfully:" << texturePath;
            } else {
                qDebug() << "Failed to load texture:" << texturePath;
                qDebug() << "Error: Image is null";
            }
        }
    }
}

void Object::render(QOpenGLShaderProgram* program) {
    if (!isInitialized || vertices.isEmpty() || indices.isEmpty()) {
        return;
    }

    for (auto it = materials.begin(); it != materials.end(); ++it) {
        if (it.value().diffuseTexture && !it.value().diffuseTexture->isCreated()) {
            qDebug() << "Warning: Texture is not created for material:" << it.key();
            continue;
        }
    }

    vbo.bind();
    ibo.bind();

    program->enableAttributeArray("position");
    program->enableAttributeArray("normal");
    program->enableAttributeArray("texCoord");
    program->enableAttributeArray("tangent");
    program->enableAttributeArray("bitangent");

    program->setAttributeBuffer("position", GL_FLOAT, 0, 3, sizeof(Vertex));
    program->setAttributeBuffer("normal", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));
    program->setAttributeBuffer("texCoord", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));
    program->setAttributeBuffer("tangent", GL_FLOAT, offsetof(Vertex, tangent), 3, sizeof(Vertex));
    program->setAttributeBuffer("bitangent", GL_FLOAT, offsetof(Vertex, bitangent), 3, sizeof(Vertex));

    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_LINE_SMOOTH);

    program->disableAttributeArray("position");
    program->disableAttributeArray("normal");
    program->disableAttributeArray("texCoord");
    program->disableAttributeArray("tangent");
    program->disableAttributeArray("bitangent");

    ibo.release();
    vbo.release();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error in render:" << err;
    }
}

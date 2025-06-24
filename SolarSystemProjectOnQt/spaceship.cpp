#include "spaceship.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>

SpaceShip::SpaceShip(QObject *parent)
    : QObject(parent)
    , m_program(nullptr)
    , m_texture(nullptr)
    , m_vbo(QOpenGLBuffer::VertexBuffer)
    , m_ibo(QOpenGLBuffer::IndexBuffer)
    , m_position(0.0f, 0.0f, 15.0f)
    , m_rotationAngle(0.0f)
    , m_speed(10.0f)
    , m_tiltAngleX(0.0f)
    , m_tiltAngleZ(0.0f)
    , m_maxTiltAngle(30.0f)
    , m_tiltSpeed(30.0f)
    , m_movingForward(false)
    , m_movingBackward(false)
    , m_movingLeft(false)
    , m_movingRight(false)
    , m_movingUp(false)
    , m_movingDown(false)
{
    qDebug() << "Initializing spaceship...";
    initializeGL();
}

void SpaceShip::initializeGL()
{
    qDebug() << "Initializing OpenGL for spaceship...";

    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram(this);

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/spaceship_vertex.vsh")) {
        qDebug() << "Failed to load vertex shader:" << m_program->log();
        return;
    }

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/spaceship_fragment.fsh")) {
        qDebug() << "Failed to load fragment shader:" << m_program->log();
        return;
    }

    if (!m_program->link()) {
        qDebug() << "Failed to link shader program:" << m_program->log();
        return;
    }

    if (!loadModel(":/spaceship.obj")) {
        qDebug() << "Failed to load spaceship model";
        return;
    }

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_vertices.constData(), m_vertices.size() * sizeof(Vertex));

    m_ibo.create();
    m_ibo.bind();
    m_ibo.allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));

    setupVertexAttribs();

    m_texture = new QOpenGLTexture(QImage(":/spaceship_normal.png").mirrored());
    if (!m_texture->isCreated()) {
        qDebug() << "Failed to create texture";
        return;
    }

    m_vao.release();
    m_vbo.release();
    m_ibo.release();

    qDebug() << "Spaceship initialization complete";
    qDebug() << "Number of vertices:" << m_vertices.size();
    qDebug() << "Number of indices:" << m_indices.size();
    qDebug() << "Initial position:" << m_position;
}

SpaceShip::~SpaceShip()
{
    if (m_program) {
        delete m_program;
    }
    if (m_texture) {
        delete m_texture;
    }
    m_vbo.destroy();
    m_vao.destroy();
}

void SpaceShip::render(const QMatrix4x4 &viewMatrix, const QMatrix4x4 &projectionMatrix)
{
    qDebug() << "Rendering spaceship at position:" << m_position;
    qDebug() << "Number of vertices:" << m_vertices.size();
    qDebug() << "Number of indices:" << m_indices.size();

    m_program->bind();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error before setting uniforms:" << err;
    }

    QMatrix4x4 modelMatrix;
    modelMatrix.translate(m_position);
    modelMatrix.rotate(m_rotationAngle, 0.0f, 1.0f, 0.0f);
    modelMatrix.rotate(m_tiltAngleX, 1.0f, 0.0f, 0.0f);
    modelMatrix.rotate(m_tiltAngleZ, 0.0f, 0.0f, 1.0f);
    modelMatrix.scale(0.005f);

    qDebug() << "Model matrix:" << modelMatrix;

    m_program->setUniformValue("model", modelMatrix);
    m_program->setUniformValue("view", viewMatrix);
    m_program->setUniformValue("projection", projectionMatrix);

    m_program->setUniformValue("lightPos", QVector3D(0.0f, 0.0f, 0.0f));
    m_program->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    m_program->setUniformValue("objectColor", QVector3D(0.8f, 0.8f, 0.8f));
    m_program->setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 10.0f));
    m_program->setUniformValue("useNormalMap", true);

    if (m_texture) {
        m_texture->bind();
        m_program->setUniformValue("textureSampler", 0);
        qDebug() << "Texture bound successfully";
    } else {
        qDebug() << "Texture not bound - not created";
    }

    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error before drawing:" << err;
    }

    m_vao.bind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_BLEND);

    m_vao.release();

    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error after drawing:" << err;
    }

    m_program->release();
}

void SpaceShip::update(float deltaTime)
{
    QVector3D movement(0.0f, 0.0f, 0.0f);

    float rotationSpeed = 90.0f;
    float fixedDeltaTime = 0.03f;
    
    if (m_movingLeft) {
        m_rotationAngle += rotationSpeed * fixedDeltaTime;
    }
    if (m_movingRight) {
        m_rotationAngle -= rotationSpeed * fixedDeltaTime;
    }

    if (m_movingForward) movement.setZ(1.0f);
    if (m_movingBackward) movement.setZ(-1.0f);
    if (m_movingUp) movement.setY(1.0f);
    if (m_movingDown) movement.setY(-1.0f);

    if (!movement.isNull()) {
        movement.normalize();
    }

    QMatrix4x4 rotationMatrix;
    rotationMatrix.rotate(m_rotationAngle, 0.0f, 1.0f, 0.0f);
    movement = rotationMatrix.mapVector(movement);

    m_position += movement * m_speed * deltaTime;

    float targetTiltX = 0.0f;
    float targetTiltZ = 0.0f;

    if (m_movingForward) targetTiltX = -m_maxTiltAngle;
    if (m_movingBackward) targetTiltX = m_maxTiltAngle;
    if (m_movingLeft) targetTiltZ = m_maxTiltAngle;
    if (m_movingRight) targetTiltZ = -m_maxTiltAngle;

    float lerpFactor = qMin(m_tiltSpeed * fixedDeltaTime, 1.0f);
    m_tiltAngleX = m_tiltAngleX + (targetTiltX - m_tiltAngleX) * lerpFactor;
    m_tiltAngleZ = m_tiltAngleZ + (targetTiltZ - m_tiltAngleZ) * lerpFactor;

    if (!m_movingForward && !m_movingBackward) {
        m_tiltAngleX *= 0.95f;
    }
    if (!m_movingLeft && !m_movingRight) {
        m_tiltAngleZ *= 0.95f;
    }
}

bool SpaceShip::loadModel(const QString& filename)
{
    qDebug() << "Attempting to load model from:" << filename;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open model file:" << filename;
        return false;
    }

    qDebug() << "File opened successfully";
    QTextStream in(&file);
    QString line;
    int lineCount = 0;
    QVector<QVector3D> positions;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;

    while (!in.atEnd()) {
        line = in.readLine();
        lineCount++;
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts.isEmpty()) continue;

        if (parts[0] == "v") {
            if (parts.size() >= 4) {
                positions.push_back(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            }
        } else if (parts[0] == "vn") {
            if (parts.size() >= 4) {
                normals.push_back(QVector3D(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat()));
            }
        } else if (parts[0] == "vt") {
            if (parts.size() >= 3) {
                texCoords.push_back(QVector2D(parts[1].toFloat(), parts[2].toFloat()));
            }
        }
    }

    qDebug() << "Finished reading file. Total lines:" << lineCount;
    qDebug() << "Found" << positions.size() << "positions," << normals.size() << "normals, and" << texCoords.size() << "texture coordinates";

    file.seek(0);
    in.seek(0);
    lineCount = 0;
    QHash<QString, int> vertexIndices;
    m_vertices.clear();
    m_indices.clear();

    while (!in.atEnd()) {
        line = in.readLine();
        lineCount++;
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts.isEmpty() || parts[0] != "f") continue;

        QVector<int> faceIndices;
        for (int i = 1; i < parts.size(); ++i) {
            QStringList vertexData = parts[i].split("/");
            if (vertexData.size() < 1) continue;

            int posIndex = vertexData[0].toInt() - 1;
            int texIndex = vertexData.size() > 1 ? vertexData[1].toInt() - 1 : 0;
            int normIndex = vertexData.size() > 2 ? vertexData[2].toInt() - 1 : 0;

            Vertex vertex;
            vertex.position = positions[posIndex];
            vertex.normal = normals.size() > normIndex ? normals[normIndex] : QVector3D(0, 1, 0);
            vertex.texCoord = texCoords.size() > texIndex ? texCoords[texIndex] : QVector2D(0, 0);

            QString vertexKey = QString("%1/%2/%3").arg(posIndex).arg(texIndex).arg(normIndex);
            if (!vertexIndices.contains(vertexKey)) {
                vertexIndices[vertexKey] = m_vertices.size();
                m_vertices.push_back(vertex);
            }
            faceIndices.push_back(vertexIndices[vertexKey]);
        }

        if (faceIndices.size() >= 3) {
            // First triangle
            m_indices.push_back(faceIndices[0]);
            m_indices.push_back(faceIndices[1]);
            m_indices.push_back(faceIndices[2]);

            if (faceIndices.size() == 4) {
                m_indices.push_back(faceIndices[0]);
                m_indices.push_back(faceIndices[2]);
                m_indices.push_back(faceIndices[3]);
            }
        }
    }

    qDebug() << "Loaded model with" << m_vertices.size() << "vertices and" << m_indices.size() << "indices";
    return true;
}

bool SpaceShip::loadTexture(const QString& filename)
{
    if (m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }

    m_texture = new QOpenGLTexture(QImage(filename).mirrored());
    if (!m_texture->isCreated()) {
        qDebug() << "Failed to create texture from file:" << filename;
        delete m_texture;
        m_texture = nullptr;
        return false;
    }

    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
    m_texture->generateMipMaps();

    return true;
}

void SpaceShip::setupVertexAttribs()
{

    m_program->bind();

    m_program->enableAttributeArray("posAttr");
    m_program->setAttributeBuffer("posAttr", GL_FLOAT, offsetof(Vertex, position), 3, sizeof(Vertex));

    m_program->enableAttributeArray("normalAttr");
    m_program->setAttributeBuffer("normalAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    m_program->enableAttributeArray("texAttr");
    m_program->setAttributeBuffer("texAttr", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

    m_program->release();
}


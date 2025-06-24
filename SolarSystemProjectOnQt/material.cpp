#include "material.h"
#include <QImage>
#include <QDebug>

Material::Material(const std::string& dayTexturePath, const std::string& normalMapPath) {
    if (!dayTexturePath.empty()) {
        qDebug() << "Loading day texture:" << QString::fromStdString(dayTexturePath);
        QImage dayImage(QString::fromStdString(dayTexturePath));
        if (dayImage.isNull()) {
            qDebug() << "Failed to load day texture:" << QString::fromStdString(dayTexturePath);
        } else {
            m_dayTexture = std::make_shared<QOpenGLTexture>(dayImage);
            qDebug() << "Day texture loaded successfully";
        }
    }
    if (!normalMapPath.empty()) {
        qDebug() << "Loading normal map:" << QString::fromStdString(normalMapPath);
        QImage normalMapImage(QString::fromStdString(normalMapPath));
        if (normalMapImage.isNull()) {
            qDebug() << "Failed to load normal map:" << QString::fromStdString(normalMapPath);
        } else {
            m_normalMap = std::make_shared<QOpenGLTexture>(normalMapImage);
            qDebug() << "Normal map loaded successfully";
        }
    }
}

void Material::initialize(QOpenGLFunctions* glFunctions) {
    if (m_dayTexture) {
        m_dayTexture->create();
        m_dayTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_dayTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        m_dayTexture->setWrapMode(QOpenGLTexture::Repeat);
        m_dayTexture->generateMipMaps();
        qDebug() << "Initialized day texture";
    }
    if (m_normalMap) {
        m_normalMap->create();
        m_normalMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_normalMap->setMagnificationFilter(QOpenGLTexture::Linear);
        m_normalMap->setWrapMode(QOpenGLTexture::Repeat);
        m_normalMap->generateMipMaps();
        qDebug() << "Initialized normal map";
    }
}

void Material::bind(QOpenGLShaderProgram* program, bool useTexture, bool useNormalMap, bool useNightTexture) {
    if (useTexture && m_dayTexture && m_dayTexture->isCreated()) {
        m_dayTexture->bind(0);
        program->setUniformValue("day_texture", 0);
        qDebug() << "Bound day_texture to unit 0";
    } else {
        qDebug() << "Day texture not bound (missing or disabled)";
    }
    if (useNormalMap && m_normalMap && m_normalMap->isCreated()) {
        m_normalMap->bind(2);
        program->setUniformValue("normal_map", 2);
        qDebug() << "Bound normal_map to unit 2";
    } else {
        qDebug() << "Normal map not bound (missing or disabled)";
    }
}

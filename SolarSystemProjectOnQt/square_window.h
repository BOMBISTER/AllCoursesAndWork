#pragma once

#include <QBasicTimer>
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QTimerEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QMouseEvent>
#include <QQuaternion>
#include <memory>
#include <vector>
#include "camera3d.h"
#include "planet.h"
#include "glow_effect.h"
#include "skybox.h"
#include "ring.h"
#include "object_loader.h"
#include "spaceship.h"

class SquareWindow : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit SquareWindow(QWidget *parent = nullptr);
    ~SquareWindow() override;
    void setLightSpeed(float speed) { lightSpeed = speed; update(); }
    void setTimeScale(float scale) { m_timeScale = scale; }
    void setCameraSpeedScale(float scale) { m_cameraSpeedScale = scale; }
    void setSkyboxEnabled(bool enabled) { m_skyboxEnabled = enabled; update(); }

public slots:
    void setUseTexture(bool use) { useTexture = use; update(); }
    void setUseNormalMap(bool use) { useNormalMap = use; update(); }
    void setUseNightTexture(bool use) { useNightTexture = use; update(); }
    void setRotationEnabled(bool enabled) { rotationEnabled = enabled; update(); }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *e) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateLightPosition();
    void setTopDownCamera();
    void setDefaultCamera();
    void setSunCamera();

    GLint posAttr = -1;
    GLint normAttr = -1;
    GLint textureAttr = -1;
    GLint tangentAttr = -1;
    GLint bitangentAttr = -1;
    GLint useTextureUniform = -1;
    GLint useNormalMapUniform = -1;
    GLint useNightTextureUniform = -1;

    std::unique_ptr<QOpenGLShaderProgram> program;
    std::unique_ptr<QOpenGLShaderProgram> m_spaceshipProgram;
    std::unique_ptr<QOpenGLShaderProgram> m_sunProgram;
    std::unique_ptr<QOpenGLShaderProgram> m_skyboxProgram;
    std::unique_ptr<QOpenGLShaderProgram> m_ringProgram;
    std::unique_ptr<QOpenGLShaderProgram> m_glowProgram;
    std::vector<std::unique_ptr<Planet>> planets;
    std::unique_ptr<GlowEffect> m_sunGlow;
    std::unique_ptr<Skybox> m_skybox;
    bool m_skyboxEnabled = true;
    std::unique_ptr<Ring> m_saturnRings;
    std::unique_ptr<SpaceShip> m_spaceship;

    QMatrix4x4 projectionMatrix;

    int frame = 0;
    float lightSpeed = 0.1f;
    float currentLightAngle = 0.0f;
    bool useTexture = true;
    bool useNormalMap = true;
    bool useNightTexture = false;
    bool rotationEnabled = true;
    float m_timeScale = 1.0f;
    float m_cameraSpeedScale = 1.0f;

    QBasicTimer timer;
    QBasicTimer lightTimer;
    Camera3D* m_camera;
    QVector2D m_mousePosition;
    

    enum class CameraMode {
        Default,
        TopDown,
        Sun
    };
    CameraMode m_cameraMode = CameraMode::Default;
    QVector3D m_defaultCameraPosition;
    QQuaternion m_defaultCameraRotation;
};

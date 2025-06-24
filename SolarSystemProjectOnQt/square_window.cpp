#include "square_window.h"
#include "camera3d.h"
#include "planet.h"
#include <cmath>
#include <QDebug>
#include <QCursor>

SquareWindow::SquareWindow(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -50.0f));
    qDebug() << "SquareWindow constructed";
}

SquareWindow::~SquareWindow() {
    delete m_camera;
}

void SquareWindow::initializeGL() {

    initializeOpenGLFunctions();

    m_spaceship = std::make_unique<SpaceShip>(this);
    m_spaceship->initializeGL();

    m_camera->translate(QVector3D(0.0f, 0.0f, 10.0f));
    m_camera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    program = std::make_unique<QOpenGLShaderProgram>(this);
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.fsh");
    if (!program->link()) {
        qDebug() << "Shader program failed to link:" << program->log();
    }

    m_sunProgram = std::make_unique<QOpenGLShaderProgram>(this);
    m_sunProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex_shader.vsh");
    m_sunProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sun_fragment.fsh");
    if (!m_sunProgram->link()) {
        qDebug() << "Sun shader program failed to link:" << m_sunProgram->log();
    }

    m_spaceshipProgram = std::make_unique<QOpenGLShaderProgram>(this);
    m_spaceshipProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/spaceship_vertex.vsh");
    m_spaceshipProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/spaceship_fragment.fsh");
    if (!m_spaceshipProgram->link()) {
        qDebug() << "Spaceship shader program failed to link:" << m_spaceshipProgram->log();
    }

    posAttr = program->attributeLocation("posAttr");
    normAttr = program->attributeLocation("normAttr");
    textureAttr = program->attributeLocation("texAttr");
    tangentAttr = program->attributeLocation("tangentAttr");
    bitangentAttr = program->attributeLocation("bitangentAttr");
    useTextureUniform = program->uniformLocation("useTexture");
    useNormalMapUniform = program->uniformLocation("useNormalMap");
    useNightTextureUniform = program->uniformLocation("useNightTexture");

    planets.push_back(std::make_unique<Planet>(3.0f, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, ":/sun_map.png", "", "Sun"));
    planets.push_back(std::make_unique<Planet>(0.5f, QVector3D(3.9f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 3.9f, 24.0f, ":/mercury_map.png", ":/mercury_normal.png", "Mercury"));
    planets.push_back(std::make_unique<Planet>(1.0f, QVector3D(7.2f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 7.2f, 62.0f, ":/venus_map.png", ":/venus_normal.png", "Venus"));
    planets.push_back(std::make_unique<Planet>(1.5f, QVector3D(10.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 10.0f, 100.0f, ":/day_map.png", ":/normal_map.png", "Earth"));
    planets.push_back(std::make_unique<Planet>(0.4f, QVector3D(11.5f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 2.5f, 10.0f, ":/mercury_map.png", ":/mercury_normal.png", "Moon"));
    planets.push_back(std::make_unique<Planet>(0.8f, QVector3D(15.2f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 15.2f, 188.0f, ":/mars_map.png", "mars_normal.png", "Mars"));
    planets.push_back(std::make_unique<Planet>(5.0f, QVector3D(52.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 52.0f, 1186.0f, ":/jupiter_map.png", ":/jupiter_normal.png", "Jupiter"));
    planets.push_back(std::make_unique<Planet>(4.0f, QVector3D(95.8f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 95.8f, 2946.0f, ":/saturn_map.png", "saturn_normal.png", "Saturn"));
    planets.push_back(std::make_unique<Planet>(2.0f, QVector3D(191.8f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 191.8f, 8400.0f, ":/uranus_map.png", "uranus_normal.png", "Uranus"));
    planets.push_back(std::make_unique<Planet>(2.0f, QVector3D(300.7f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f), 300.7f, 16480.0f, ":/neptune_map.png", ":/neptune_normal.png", "Neptune"));

    qDebug() << "Initialized" << planets.size() << "planets";
    for (auto& planet : planets) {
        qDebug() << "Planet:" << planet->name() << " at initial position" << planet->position();
        planet->initialize(this);
    }

    m_sunGlow = std::make_unique<GlowEffect>();
    m_sunGlow->initialize(this);

    m_skybox = std::make_unique<Skybox>();
    m_skybox->initialize(this);

    m_saturnRings = std::make_unique<Ring>();
    m_saturnRings->initialize();

    glEnable(GL_DEPTH_TEST);

    timer.start(30, this);
    lightTimer.start(30, this);
    QMatrix4x4 tmp;
    tmp.setToIdentity();
    m_camera->setGlobalTransform(tmp);
    qDebug() << "OpenGL Version:" << QString((const char*)glGetString(GL_VERSION));
    qDebug() << "OpenGL initialized";

    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -50.0f));
    m_defaultCameraPosition = QVector3D(0.0f, 0.0f, -50.0f);
    m_defaultCameraRotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f);
}

void SquareWindow::paintGL() {
    glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_skyboxEnabled) {
        qDebug() << "Rendering skybox";
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        m_skybox->render(this, m_camera->viewMatrix(), projectionMatrix);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    qDebug() << "Rendering Sun glow";
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    m_sunGlow->render(this, m_camera->viewMatrix(), projectionMatrix, QVector3D(0.0f, 0.0f, 0.0f), 5.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    if (m_spaceship) {
        qDebug() << "Starting spaceship rendering...";

        m_spaceshipProgram->bind();

        m_spaceshipProgram->setUniformValue("viewMatrix", m_camera->viewMatrix());
        m_spaceshipProgram->setUniformValue("projectionMatrix", projectionMatrix);
        m_spaceshipProgram->setUniformValue("lightPos", QVector3D(0.0f, 0.0f, 0.0f));
        m_spaceshipProgram->setUniformValue("viewPos", m_camera->position());

        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        qDebug() << "Calling spaceship render...";
        m_spaceship->render(m_camera->viewMatrix(), projectionMatrix);
        qDebug() << "Spaceship render completed";

        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_LINE_SMOOTH);

        m_spaceshipProgram->release();
    }

    program->bind();
    m_camera->draw(program.get(), nullptr);

    QVector3D lightPos = QVector3D(0.0f, 0.0f, 0.0f);
    program->setUniformValue("projectionMatrix", projectionMatrix);
    program->setUniformValue("viewMatrix", m_camera->viewMatrix());
    program->setUniformValue("viewPos", m_camera->position());
    program->setUniformValue("lightPos", lightPos);
    program->setUniformValue("useTexture", useTexture ? 1 : 0);
    program->setUniformValue("useNormalMap", useNormalMap ? 1 : 0);
    program->setUniformValue("useNightTexture", useNightTexture ? 1 : 0);
    QMatrix3x3 normalMatrix = m_camera->viewMatrix().normalMatrix();
    program->setUniformValue("normMatrix", normalMatrix);

    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    for (auto& planet : planets) {
        if (planet->name() == "Moon") {
            auto earth_it = std::find_if(planets.begin(), planets.end(),
                                         [](const auto& p) { return p->name() == "Earth"; });
            if (earth_it != planets.end()) {
                QVector3D earthPos = (*earth_it)->position();
                float moonOrbitAngle = planet->orbitAngle();
                QVector3D moonOffset = QVector3D(cos(moonOrbitAngle), 0.0f, sin(moonOrbitAngle)) * planet->orbitRadius();
                planet->m_position = earthPos + moonOffset;
            }
        }

        if (planet->name() == "Sun") {
            program->release();
            m_sunProgram->bind();
            m_sunProgram->setUniformValue("projectionMatrix", projectionMatrix);
            m_sunProgram->setUniformValue("viewMatrix", m_camera->viewMatrix());
            m_sunProgram->setUniformValue("viewPos", m_camera->position());
            m_sunProgram->setUniformValue("lightPos", lightPos);
            m_sunProgram->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
            m_sunProgram->setUniformValue("objectColor", QVector3D(1.0f, 1.0f, 1.0f));
            m_sunProgram->setUniformValue("useTexture", useTexture ? 1 : 0);
            m_sunProgram->setUniformValue("useNormalMap", useNormalMap ? 1 : 0);
            m_sunProgram->setUniformValue("useNightTexture", useNightTexture ? 1 : 0);
            planet->render(m_sunProgram.get(), posAttr, normAttr, textureAttr, tangentAttr, bitangentAttr, useTexture, useNormalMap, useNightTexture);
            m_sunProgram->release();
            program->bind();
        } else {
            planet->render(program.get(), posAttr, normAttr, textureAttr, tangentAttr, bitangentAttr, useTexture, useNormalMap, useNightTexture);
        }
    }

    if (posAttr != -1) program->disableAttributeArray(posAttr);
    if (normAttr != -1) program->disableAttributeArray(normAttr);
    if (textureAttr != -1) program->disableAttributeArray(textureAttr);
    if (tangentAttr != -1) program->disableAttributeArray(tangentAttr);
    if (bitangentAttr != -1) program->disableAttributeArray(bitangentAttr);

    program->release();

    qDebug() << "Rendering Saturn rings";
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto saturn_it = std::find_if(planets.begin(), planets.end(),
                                  [](const auto& p) { return p->name() == "Saturn"; });
    if (saturn_it != planets.end()) {
        QVector3D saturnPos = (*saturn_it)->position();
        qDebug() << "Saturn position:" << saturnPos;
        m_saturnRings->render(m_camera->viewMatrix(), projectionMatrix, saturnPos, QVector3D(0.0f, 0.0f, 1.0f));
    } else {
        qDebug() << "Saturn not found in planets list";
    }
    glDisable(GL_BLEND);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        qDebug() << "OpenGL error in paintGL:" << err;
    }

    ++frame;
}

void SquareWindow::resizeGL(int w, int h) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(60.0f, w / static_cast<float>(h), 0.1f, 2000.0f);
    qDebug() << "ResizeGL: w =" << w << ", h =" << h;
}

void SquareWindow::timerEvent(QTimerEvent *e) {
    if (e->timerId() == timer.timerId()) {
        float deltaTime = 0.03f * m_timeScale;

        if (m_cameraMode == CameraMode::TopDown && m_spaceship) {
            QVector3D shipPos = m_spaceship->position();
            QMatrix4x4 transform;
            transform.translate(QVector3D(shipPos.x(), shipPos.y() + 5.0f, shipPos.z()));
            transform.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), -90.0f));
            m_camera->setGlobalTransform(transform);
            qDebug() << "Updating top-down camera position:" << shipPos;
        }

        if (rotationEnabled) {
            for (auto& planet : planets) {
                planet->updateOrbit(deltaTime);
                if (planet->name() != "Sun") {
                    if (planet->name() == "Mercury" || planet->name() == "Venus" ||
                        planet->name() == "Earth" || planet->name() == "Mars" || planet->name() == "Moon") {
                        planet->rotate(0.5f * deltaTime);
                    } else {
                        planet->rotate(2.0f * deltaTime);
                    }
                }
            }
        }

        if (m_spaceship) {
            m_spaceship->update(0.03f);
        }

        update();
    } else if (e->timerId() == lightTimer.timerId()) {
        updateLightPosition();
    }
}

void SquareWindow::updateLightPosition() {
    if (fabs(lightSpeed) > 1e-6f) {
        currentLightAngle -= lightSpeed;
        currentLightAngle = fmod(currentLightAngle, 2.0f * M_PI);
    }
}

void SquareWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePosition = QVector2D(event->position());
        setFocus();
        grabMouse();
        setCursor(Qt::BlankCursor);
        qDebug() << "Mouse captured at position:" << m_mousePosition;
    }
    event->accept();
}

void SquareWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QVector2D diff = QVector2D(event->position()) - m_mousePosition;
        m_mousePosition = QVector2D(event->position());

        float angle = diff.length() / 2.0f;
        QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

        m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angle));
        update();
    }
    event->accept();
}

void SquareWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        releaseMouse();
        setCursor(Qt::ArrowCursor);
        qDebug() << "Mouse released";
    }
    event->accept();
}

void SquareWindow::keyPressEvent(QKeyEvent *event) {
    float baseShift = 1.0f;
    float shift = baseShift * m_cameraSpeedScale;
    switch (event->key()) {
    case Qt::Key_W:
        if (m_spaceship) {
            m_spaceship->setMovingUp(true);
        }
        break;
    case Qt::Key_S:
        if (m_spaceship) {
            m_spaceship->setMovingDown(true);
        }
        break;
    case Qt::Key_A:
        if (m_spaceship) {
            m_spaceship->setMovingLeft(true);
        }
        break;
    case Qt::Key_D:
        if (m_spaceship) {
            m_spaceship->setMovingRight(true);
        }
        break;
    case Qt::Key_Space:
        if (m_spaceship) {
            m_spaceship->setMovingForward(true);
        }
        break;
    case Qt::Key_Backspace:
        if (m_spaceship) {
            m_spaceship->setMovingBackward(true);
        }
        break;
    case Qt::Key_Control:
        m_camera->translate(QVector3D(0.0f, -shift, 0.0f));
        break;
    case Qt::Key_Up:
        m_camera->translate(QVector3D(0.0f, 0.0f, shift));
        break;
    case Qt::Key_Down:
        m_camera->translate(QVector3D(0.0f, 0.0f, -shift));
        break;
    case Qt::Key_Left:
        m_camera->translate(QVector3D(shift, 0.0f, 0.0f));
        break;
    case Qt::Key_Right:
        m_camera->translate(QVector3D(-shift, 0.0f, 0.0f));
        break;
    case Qt::Key_1:
        setTopDownCamera();
        break;
    case Qt::Key_2:
        setDefaultCamera();
        break;
    case Qt::Key_3:
        setSunCamera();
        break;
    }
    update();
    event->accept();
}

void SquareWindow::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_W:
        if (m_spaceship) {
            m_spaceship->setMovingUp(false);
        }
        break;
    case Qt::Key_S:
        if (m_spaceship) {
            m_spaceship->setMovingDown(false);
        }
        break;
    case Qt::Key_A:
        if (m_spaceship) {
            m_spaceship->setMovingLeft(false);
        }
        break;
    case Qt::Key_D:
        if (m_spaceship) {
            m_spaceship->setMovingRight(false);
        }
        break;
    case Qt::Key_Space:
        if (m_spaceship) {
            m_spaceship->setMovingForward(false);
        }
        break;
    case Qt::Key_Backspace:
        if (m_spaceship) {
            m_spaceship->setMovingBackward(false);
        }
        break;
    }
    event->accept();
}

void SquareWindow::setTopDownCamera() {
    if (m_cameraMode != CameraMode::TopDown) {
        m_cameraMode = CameraMode::TopDown;
        if (m_spaceship) {
            QVector3D shipPos = m_spaceship->position();
            QMatrix4x4 transform;
            transform.translate(QVector3D(shipPos.x(), shipPos.y() + 5.0f, shipPos.z()));
            transform.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), -90.0f));
            m_camera->setGlobalTransform(transform);
            qDebug() << "Top-down camera set at position:" << shipPos;
        }
    }
}

void SquareWindow::setDefaultCamera() {
    if (m_cameraMode != CameraMode::Default) {
        m_cameraMode = CameraMode::Default;
        m_camera->setGlobalTransform(QMatrix4x4());
        m_camera->translate(m_defaultCameraPosition);
        m_camera->rotate(m_defaultCameraRotation);
    }
}

void SquareWindow::setSunCamera() {
    if (m_cameraMode != CameraMode::Sun) {
        m_cameraMode = CameraMode::Sun;
        m_camera->setGlobalTransform(QMatrix4x4());
        m_camera->translate(QVector3D(0.0f, 0.0f, -20.0f));
        m_camera->rotate(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), 0.0f));
    }
}

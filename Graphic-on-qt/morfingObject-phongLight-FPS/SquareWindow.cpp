#include "SquareWindow.h"
#include <QColorDialog>
#include <QScreen>
#include <QtMath>

SquareWindow::SquareWindow(QWidget *parent) : QOpenGLWidget(parent) {
    n = 10;
    fpsTimer.start();
}

void SquareWindow::initShaders(bool useFragmentLighting) {
    program_ = std::make_unique<QOpenGLShaderProgram>(this);
    if (useFragmentLighting) {
        program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/phong_vertex_full.vsh");
        program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/phong_fragment_full.fsh");
    } else {
        program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/phong_vertex.vsh");
        program_->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/phong_fragment.fsh");
    }
    program_->link();

    posAttr_ = program_->attributeLocation("posAttr");
    normAttr_ = program_->attributeLocation("normAttr");
    Q_ASSERT(posAttr_ != -1);
}

void SquareWindow::initializeGL() {
    initializeOpenGLFunctions();
    initShaders(useFragmentLighting);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    timer.start(30, this);

    init_cube(1.5f, n);
}

void SquareWindow::paintGL() {
    const auto retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!program_ || !program_->isLinked()) {
        qDebug() << "Shader program is not linked, skipping rendering.";
        return;
    }

    qDebug() << "Rendering frame:" << frame_;
    qDebug() << "Light type:" << lightType << ", Light direction:" << lightDirection << ", Light position:" << QVector3D(light_x_param, light_y_param, light_z_param);

    program_->bind();

    QMatrix4x4 view_matrix;
    view_matrix.setToIdentity();

    program_->setUniformValue("projection_matrix", projection_matrix);
    program_->setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 0.0f));
    program_->setUniformValue("lightPos", QVector3D(light_x_param, light_y_param, light_z_param));
    program_->setUniformValue("objectColor", cube_color);
    program_->setUniformValue("lightColor", lightColor);
    program_->setUniformValue("morph_param", morph_param);
    program_->setUniformValue("lightType", lightType);
    program_->setUniformValue("lightIntensity", lightIntensity);
    program_->setUniformValue("lightAttenuation", lightAttenuation);
    program_->setUniformValue("lightDirection", lightDirection);
    program_->setUniformValue("lightCutoff", lightCutoff);
    program_->setUniformValue("outerCutoff", outerCutoff); // Передаём внешний угол

    vertexBuffer.bind();
    indexBuffer.bind();
    program_->setAttributeBuffer(posAttr_, GL_FLOAT, 0, 3, sizeof(VertexData));
    program_->enableAttributeArray(posAttr_);
    program_->setAttributeBuffer(normAttr_, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData));
    program_->enableAttributeArray(normAttr_);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; ++j) {
            QMatrix4x4 model_matrix;
            model_matrix.setToIdentity();
            model_matrix.translate(-2.5 + i * 2.5, -2.5 + j * 2.5, -8.0f);
            model_matrix.rotate(100.0 * frame_ / 59.0, rotationAxis);

            program_->setUniformValue("model", model_matrix);
            program_->setUniformValue("view_matrix", view_matrix);
            QMatrix4x4 modelViewMatrix = view_matrix * model_matrix;
            program_->setUniformValue("norm_matrix", modelViewMatrix.normalMatrix());

            glDrawElements(GL_TRIANGLES, indexBuffer.size() / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);
            qDebug() << "Drawing" << (indexBuffer.size() / sizeof(GLuint)) << "indices";
        }
    }

    program_->disableAttributeArray(posAttr_);
    program_->disableAttributeArray(normAttr_);
    vertexBuffer.release();
    indexBuffer.release();
    program_->release();
    ++frame_;

    // Расчёт FPS
    frameCount++;
    qint64 elapsed = fpsTimer.elapsed();
    qDebug() << "Elapsed time:" << elapsed << "ms, Frame count:" << frameCount;
    if (elapsed >= 1000) {
        currentFps = frameCount * 1000 / elapsed;
        qDebug() << "FPS calculated:" << currentFps;
        emit fpsUpdated(currentFps);
        frameCount = 0;
        fpsTimer.restart();
    }
}

void SquareWindow::resizeGL(const int w, const int h) {
    const auto aspect = w / static_cast<double>(h);
    projection_matrix.setToIdentity();
    projection_matrix.perspective(60.0f, aspect, 0.01f, 100.0f);
}

void SquareWindow::change_morph_param(float value) {
    morph_param = value / 1000;
    update();
}

void SquareWindow::change_light_x_param(float value) {
    if (lightType == 0) {
        lightDirection.setX(value);
    } else {
        light_x_param = value;
    }
    update();
}

void SquareWindow::change_light_y_param(float value) {
    if (lightType == 0) {
        lightDirection.setY(value);
    } else {
        light_y_param = value;
    }
    update();
}

void SquareWindow::change_light_z_param(float value) {
    if (lightType == 0) {
        lightDirection.setZ(value);
    } else {
        light_z_param = value;
    }
    update();
}

void SquareWindow::mousePressEvent(QMouseEvent *e) {
    mousePressPosition = QVector2D(e->position());
}

void SquareWindow::mouseReleaseEvent(QMouseEvent *e) {
    const auto diff = QVector2D(e->position()) - mousePressPosition;
    rotationAxis = QVector3D(diff.y(), diff.x(), 0.0).normalized();
}

void SquareWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key::Key_Plus) {
        const auto chosen_color = QColorDialog::getColor();
        cube_color =
            QVector3D(chosen_color.red() / 255.0f,
                      chosen_color.green() / 255.0f,
                      chosen_color.blue() / 255.0f);
    } else if (e->key() == Qt::Key::Key_Minus) {
        const auto chosen_color = QColorDialog::getColor();
        lightColor =
            QVector3D(chosen_color.red() / 255.0f,
                      chosen_color.green() / 255.0f,
                      chosen_color.blue() / 255.0f);
    }
}

void SquareWindow::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
    qDebug() << "Timer event triggered, requesting update.";
    update();
}

void SquareWindow::init_cube(const float width, const int N) {
    auto width_div_2 = width / 2.0f;
    auto step = width / float(N - 1);

    std::vector<VertexData> vertexes;
    vertexes.reserve(6 * pow(N, 2));
    for (auto z = -width_div_2; z <= width_div_2; z += width) {
        for (auto j = 0; j < N; j++) {
            for (auto i = 0; i < N; i++) {
                vertexes.emplace_back(
                    VertexData(QVector3D(-z + i * step * z / width_div_2,
                                         -width_div_2 + j * step, z),
                               QVector3D(0.0, 0.0, z / width_div_2)));
            }
        }
    }
    for (auto x = -width_div_2; x <= width_div_2; x += width) {
        for (auto k = 0; k < N; k++) {
            for (auto j = 0; j < N; j++) {
                vertexes.emplace_back(
                    VertexData(QVector3D(x, -width_div_2 + j * step,
                                         -x + x * k * step / width_div_2),
                               QVector3D(x / width_div_2, 0.0, 0.0)));
            }
        }
    }
    for (auto y = -width_div_2; y <= width_div_2; y += width) {
        for (auto i = 0; i < N; i++) {
            for (auto k = 0; k < N; k++) {
                vertexes.emplace_back(
                    VertexData(QVector3D(-width_div_2 + i * step, y,
                                         -y + y * k * step / width_div_2),
                               QVector3D(0.0, y / width_div_2, 0.0)));
            }
        }
    }

    std::vector<GLuint> indexes;
    int vertex_count = 36 * pow(N - 1, 2);
    indexes.reserve(vertex_count);
    for (int i = 0; i < 6 * N * N; i += N * N) {
        for (int j = 0; j < (N - 1) * (N - 1); j += N) {
            for (int k = 0; k < (N - 1); k++) {
                indexes.emplace_back(i + j + k + N);
                indexes.emplace_back(i + j + k + 0);
                indexes.emplace_back(i + j + k + N + 1);
                indexes.emplace_back(i + j + k + N + 1);
                indexes.emplace_back(i + j + k + 0);
                indexes.emplace_back(i + j + k + 1);
            }
        }
    }

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexes.data(),
                          static_cast<int>(vertexes.size() * sizeof(VertexData)));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indexes.data(),
                         static_cast<int>(indexes.size() * sizeof(GLuint)));
}

void SquareWindow::change_cube_color() {
    const auto chosen_color = QColorDialog::getColor();
    cube_color =
        QVector3D(chosen_color.red() / 255.0f,
                  chosen_color.green() / 255.0f,
                  chosen_color.blue() / 255.0f);
    update();
}

void SquareWindow::change_light_color() {
    const auto chosen_color = QColorDialog::getColor();
    lightColor =
        QVector3D(chosen_color.red() / 255.0f,
                  chosen_color.green() / 255.0f,
                  chosen_color.blue() / 255.0f);
    update();
}

void SquareWindow::changeN(int new_n) {
    n = new_n;
    init_cube(1.5f, n);
    update();
}

void SquareWindow::setLightType(int type) {
    lightType = type;

    if (lightType == 0) {
        lightIntensity = 1.0f;
        lightAttenuation = 0.0f;
        lightCutoff = 0.0f;
        outerCutoff = 0.0f;
        lightDirection = QVector3D(1.0f, 1.0f, 1.0f).normalized();
        light_x_param = lightDirection.x();
        light_y_param = lightDirection.y();
        light_z_param = lightDirection.z();
    } else if (lightType == 1) {
        lightIntensity = 10.0f;
        lightAttenuation = 0.01f;
        lightCutoff = 0.0f;
        outerCutoff = 0.0f;
        lightDirection = QVector3D(0.0f, 0.0f, 0.0f);
        light_x_param = 1.0f;
        light_y_param = 1.0f;
        light_z_param = 1.0f;
    } else if (lightType == 2) {
        lightIntensity = 10.0f;
        lightAttenuation = 0.01f;
        lightCutoff = 0.9f;
        outerCutoff = 0.8f;
        lightDirection = QVector3D(1.0f, 1.0f, 1.0f).normalized();
        light_x_param = 1.0f;
        light_y_param = 1.0f;
        light_z_param = 1.0f;
    }

    update();
}

void SquareWindow::setLightIntensity(float intensity) {
    lightIntensity = intensity;
    update();
}

void SquareWindow::setLightAttenuation(float attenuation) {
    lightAttenuation = attenuation / 1000.0f;
    update();
}

void SquareWindow::setLightCutoff(float cutoff) {
    lightCutoff = cutoff / 100.0f;
    update();
}

void SquareWindow::setOuterCutoff(float cutoff) {
    outerCutoff = cutoff / 100.0f;
    update();
}

void SquareWindow::setLightDirection(QVector3D direction) {
    lightDirection = direction;
    update();
}

void SquareWindow::setLightingMode(bool useFragmentLighting) {
    this->useFragmentLighting = useFragmentLighting;
    initShaders(useFragmentLighting);
    update();
}

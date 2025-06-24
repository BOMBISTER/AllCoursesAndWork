#include "main_window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    squareWindow = new SquareWindow(this);

    lightSpeedSlider = new QSlider(Qt::Horizontal, this);
    lightSpeedSlider->setRange(0, 200);
    lightSpeedSlider->setValue(100);
    lightSpeedSlider->setFixedWidth(300);
    lightSpeedSlider->setTickPosition(QSlider::TicksBelow);
    lightSpeedSlider->setTickInterval(20);

    timeScaleSlider = new QSlider(Qt::Horizontal, this);
    timeScaleSlider->setRange(0, 100); // 0x to 10x
    timeScaleSlider->setValue(10); // Default 1x
    timeScaleSlider->setFixedWidth(300);
    timeScaleSlider->setTickPosition(QSlider::TicksBelow);
    timeScaleSlider->setTickInterval(10);

    cameraSpeedSlider = new QSlider(Qt::Horizontal, this);
    cameraSpeedSlider->setRange(1, 100); // 0.1x to 10x
    cameraSpeedSlider->setValue(10); // Default 1x
    cameraSpeedSlider->setFixedWidth(300);
    cameraSpeedSlider->setTickPosition(QSlider::TicksBelow);
    cameraSpeedSlider->setTickInterval(10);

    textureCheckBox = new QCheckBox("Texture", this);
    textureCheckBox->setChecked(true);
    normalMapCheckBox = new QCheckBox("Normal map", this);
    normalMapCheckBox->setChecked(true);
    nightTextureCheckBox = new QCheckBox("Night texture", this);
    nightTextureCheckBox->setChecked(false);
    rotateCheckBox = new QCheckBox("Rotate planet", this);
    rotateCheckBox->setChecked(true);
    skyboxCheckBox = new QCheckBox("Show stars", this);
    skyboxCheckBox->setChecked(true);

    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->addWidget(new QLabel("Light Speed:", this));
    controlLayout->addWidget(lightSpeedSlider);
    controlLayout->addWidget(new QLabel("Time Scale:", this));
    controlLayout->addWidget(timeScaleSlider);
    controlLayout->addWidget(new QLabel("Camera Speed:", this));
    controlLayout->addWidget(cameraSpeedSlider);
    controlLayout->addWidget(textureCheckBox);
    controlLayout->addWidget(normalMapCheckBox);
    controlLayout->addWidget(nightTextureCheckBox);
    controlLayout->addWidget(rotateCheckBox);
    controlLayout->addWidget(skyboxCheckBox);
    controlLayout->addStretch();

    QWidget *controlPanel = new QWidget(this);
    controlPanel->setLayout(controlLayout);
    controlPanel->setMinimumHeight(100);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(squareWindow, 1);
    mainLayout->addWidget(controlPanel, 0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(mainLayout);

    connect(lightSpeedSlider, &QSlider::valueChanged, this, [=](int value) {
        float speed = value * 0.01f;
        squareWindow->setLightSpeed(speed / 10.0f);
    });

    connect(timeScaleSlider, &QSlider::valueChanged, this, [=](int value) {
        float scale = value / 10.0f;
        squareWindow->setTimeScale(scale);
    });

    connect(cameraSpeedSlider, &QSlider::valueChanged, this, [=](int value) {
        float scale = value / 10.0f;
        squareWindow->setCameraSpeedScale(scale);
    });

    connect(textureCheckBox, &QCheckBox::toggled, squareWindow, &SquareWindow::setUseTexture);
    connect(normalMapCheckBox, &QCheckBox::toggled, squareWindow, &SquareWindow::setUseNormalMap);
    connect(nightTextureCheckBox, &QCheckBox::toggled, squareWindow, &SquareWindow::setUseNightTexture);
    connect(rotateCheckBox, &QCheckBox::toggled, squareWindow, &SquareWindow::setRotationEnabled);
    connect(skyboxCheckBox, &QCheckBox::toggled, squareWindow, &SquareWindow::setSkyboxEnabled);

    squareWindow->setLightSpeed(0.1f);

    setWindowTitle("Display");
    resize(800, 600);
}

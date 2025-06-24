#include "MyWidget.h"

template <typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
using main = T;

auto slider(int min_value, int max_value, int tick_value) {
    auto *const slider = main<QSlider*>(new QSlider(Qt::Orientation::Horizontal));
    slider->setRange(min_value, max_value);
    slider->setSingleStep(tick_value);
    slider->setTickPosition(QSlider::TicksRight);
    slider->setValue(max_value);
    return slider;
}

MyWidget::MyWidget() {
    auto *const my_window = main<SquareWindow*>(new SquareWindow);

    auto *const container = main<QGridLayout *>(new QGridLayout);
    auto *const mainLayout = main<QVBoxLayout *>(new QVBoxLayout);
    auto *const w = main<QWidget *>(new QWidget);

    tSpinBox = new QSpinBox;
    tSpinBox->setRange(2, 10000);
    tSpinBox->setValue(10);
    tSpinBox->setSingleStep(1);

    sun_Slider_x = slider(-100, 100, 5);
    sun_Slider_y = slider(-100, 100, 5);
    sun_Slider_z = slider(-100, 100, 5);
    morphSlider = slider(0, 1000, 10);

    nLabel = new QLabel("Grid Density (n):");
    lightXLabel = new QLabel("Light X:");
    lightYLabel = new QLabel("Light Y:");
    lightZLabel = new QLabel("Light Z:");
    morphLabel = new QLabel("Morphing:");
    directionalLightRadio = new QRadioButton("Directional Light");
    pointLightRadio = new QRadioButton("Point Light");
    spotlightRadio = new QRadioButton("Spotlight");
    directionalLightRadio->setChecked(true);
    intensitySlider = slider(1, 100, 1);
    attenuationSlider = slider(1, 100, 1);
    cutoffSlider = slider(70, 100, 1);
    outerCutoffSlider = slider(50, 90, 1);

    fpsLabel = new QLabel("FPS: 0");
    fpsLabel->setAlignment(Qt::AlignRight);

    QPushButton *change_color_cube_button = new QPushButton(tr("&CubeColor"));
    change_color_cube_button->setFocusPolicy(Qt::NoFocus);

    QPushButton *change_color_light_button = new QPushButton(tr("&LightColor"));
    change_color_light_button->setFocusPolicy(Qt::NoFocus);

    toggleLightingButton = new QPushButton(tr("Toggle Lighting (Vertex)"));
    toggleLightingButton->setFocusPolicy(Qt::NoFocus);

    container->addWidget(my_window, 0, 0, 1, 2);
    container->addWidget(fpsLabel, 0, 2);
    container->addWidget(nLabel, 1, 0);
    container->addWidget(tSpinBox, 1, 1);
    container->addWidget(lightXLabel, 2, 0);
    container->addWidget(sun_Slider_x, 2, 1);
    container->addWidget(lightYLabel, 3, 0);
    container->addWidget(sun_Slider_y, 3, 1);
    container->addWidget(lightZLabel, 4, 0);
    container->addWidget(sun_Slider_z, 4, 1);
    container->addWidget(morphLabel, 5, 0);
    container->addWidget(morphSlider, 5, 1);
    container->addWidget(change_color_cube_button, 6, 0);
    container->addWidget(change_color_light_button, 6, 1);
    container->addWidget(new QLabel("Light Type:"), 7, 0);
    container->addWidget(directionalLightRadio, 7, 1);
    container->addWidget(pointLightRadio, 8, 0);
    container->addWidget(spotlightRadio, 8, 1);
    container->addWidget(new QLabel("Intensity:"), 9, 0);
    container->addWidget(intensitySlider, 9, 1);
    container->addWidget(new QLabel("Attenuation:"), 10, 0);
    container->addWidget(attenuationSlider, 10, 1);
    container->addWidget(new QLabel("Inner Cutoff:"), 11, 0);
    container->addWidget(cutoffSlider, 11, 1);
    container->addWidget(new QLabel("Outer Cutoff:"), 12, 0);
    container->addWidget(outerCutoffSlider, 12, 1);
    container->addWidget(toggleLightingButton, 13, 0, 1, 2);

    w->setLayout(container);
    mainLayout->addWidget(w);

    setLayout(mainLayout);

    connect(tSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), my_window, &SquareWindow::changeN);
    connect(sun_Slider_x, &QSlider::valueChanged, my_window, &SquareWindow::change_light_x_param);
    connect(sun_Slider_y, &QSlider::valueChanged, my_window, &SquareWindow::change_light_y_param);
    connect(sun_Slider_z, &QSlider::valueChanged, my_window, &SquareWindow::change_light_z_param);
    connect(morphSlider, &QSlider::valueChanged, my_window, &SquareWindow::change_morph_param);
    connect(change_color_cube_button, &QPushButton::clicked, my_window, &SquareWindow::change_cube_color);
    connect(change_color_light_button, &QPushButton::clicked, my_window, &SquareWindow::change_light_color);
    connect(directionalLightRadio, &QRadioButton::clicked, [my_window]() {
        my_window->setLightType(0);
    });
    connect(pointLightRadio, &QRadioButton::clicked, [my_window]() {
        my_window->setLightType(1);
    });
    connect(spotlightRadio, &QRadioButton::clicked, [my_window]() {
        my_window->setLightType(2);
    });
    connect(intensitySlider, &QSlider::valueChanged, my_window, &SquareWindow::setLightIntensity);
    connect(attenuationSlider, &QSlider::valueChanged, my_window, &SquareWindow::setLightAttenuation);
    connect(cutoffSlider, &QSlider::valueChanged, my_window, &SquareWindow::setLightCutoff);
    connect(outerCutoffSlider, &QSlider::valueChanged, my_window, &SquareWindow::setOuterCutoff);

    connect(toggleLightingButton, &QPushButton::clicked, this, [this, my_window]() {
        bool useFragment = !my_window->isFragmentLighting();
        my_window->setLightingMode(useFragment);
        toggleLightingButton->setText(useFragment ? tr("Toggle Lighting (Fragment)") : tr("Toggle Lighting (Vertex)"));
    });

    connect(my_window, &SquareWindow::fpsUpdated, this, [this](int fps) {
        qDebug() << "FPS updated in MyWidget:" << fps;
        fpsLabel->setText(QString("FPS: %1").arg(fps));
    });
}

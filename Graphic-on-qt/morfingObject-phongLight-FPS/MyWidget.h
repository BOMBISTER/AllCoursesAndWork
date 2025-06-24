#pragma once
#include "SquareWindow.h"
#include <QColorDialog>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>
#include <type_traits>
#include <QPushButton>

class MyWidget : public QWidget {
    Q_OBJECT

public:
    MyWidget();



private:
    QSlider *morphSlider;
    QSpinBox *tSpinBox;
    QSlider *sun_Slider_x;
    QSlider *sun_Slider_y;
    QSlider *sun_Slider_z;
    QLabel *nLabel;
    QLabel *lightXLabel;
    QLabel *lightYLabel;
    QLabel *lightZLabel;
    QLabel *morphLabel;
    QRadioButton *directionalLightRadio;
    QRadioButton *pointLightRadio;
    QRadioButton *spotlightRadio;
    QSlider *intensitySlider;
    QSlider *attenuationSlider;
    QSlider *cutoffSlider;
    QSlider *outerCutoffSlider;
    QLabel *fpsLabel;
    QPushButton *toggleLightingButton;
};

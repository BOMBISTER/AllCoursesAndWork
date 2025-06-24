#include "scene_widget.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QSlider>
#include <QLineEdit>
#include <QIntValidator>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto main_widget = new QWidget;

    auto morphing_slider = new QSlider(Qt::Horizontal, main_widget);
    auto split_parameter_edit = new QLineEdit(main_widget);
    auto scene_widget = new Scene_widget(main_widget);

    morphing_slider->setRange(0, 100);
    QObject::connect(morphing_slider, &QAbstractSlider::valueChanged, [=](int value) {
        scene_widget->set_morphing_parameter(float(value) / 100);
    });

    split_parameter_edit->setAlignment(Qt::AlignHCenter);
    split_parameter_edit->setValidator(new QIntValidator(1, 9999, split_parameter_edit));
    QObject::connect(split_parameter_edit, &QLineEdit::textChanged, [=](const QString &text) {
        scene_widget->set_split_parameter(text.toInt());
    });
    split_parameter_edit->setText("10");

    scene_widget->setMinimumSize(640, 640);
    auto main_layout = new QVBoxLayout(main_widget);

    main_layout->addWidget(scene_widget);
    main_layout->addWidget(morphing_slider);
    main_layout->addWidget(split_parameter_edit);

    main_widget->show();

    return a.exec();
}

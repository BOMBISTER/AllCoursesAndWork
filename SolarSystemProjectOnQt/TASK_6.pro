QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera3d.cpp \
    glow_effect.cpp \
    main.cpp \
    main_window.cpp \
    material.cpp \
    object_loader.cpp \
    planet.cpp \
    ring.cpp \
    skybox.cpp \
    spaceship.cpp \
    square_window.cpp

HEADERS += \
    camera3d.h \
    glow_effect.h \
    main_window.h \
    material.h \
    object_loader.h \
    planet.h \
    ring.h \
    skybox.h \
    spaceship.h \
    square_window.h \
    transformational.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc

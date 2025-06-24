
#include "scene_widget.h"
#include <QMouseEvent>


Scene_widget::Scene_widget(QWidget *parent) : QOpenGLWidget(parent)
{
    update_geometry();

    QMatrix4x4 view;
    QMatrix4x4 projection;

    projection.perspective(60.0f, 1.0f, 0.1f, 100.0f);
    view.lookAt({0, 0, -2}, { 0, 0, 0}, { 1, 0, 0});

    m_view_projection = projection * view;

    for (size_t i = 0; i < 6; ++i) {
        m_rotations.push_back({});
    }

    m_rotations[1].rotate(90, 1, 0, 0);
    m_rotations[2].rotate(180, 1, 0, 0);
    m_rotations[3].rotate(270, 1, 0, 0);
    m_rotations[4].rotate(90, 0, 1, 0);
    m_rotations[5].rotate(270, 0, 1, 0);
}

void Scene_widget::set_split_parameter(size_t parameter)
{
    m_split_parameter = parameter;
    update_geometry();
}

void Scene_widget::set_morphing_parameter(float parameter)
{
    m_cube_renderer.setMorphingParameter(parameter);
}


void Scene_widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader_program = new QOpenGLShaderProgram(this);
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":vertex_shader.vsh");
    m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":fragment_shader.fsh");

    Q_ASSERT(m_shader_program->link());
    m_pos_attr = m_shader_program->attributeLocation("pos_attr");
    m_col_attr = m_shader_program->attributeLocation("col_attr");
    m_matrix_uniform = m_shader_program->uniformLocation("mvp_matrix");
    m_morphing_uniform = m_shader_program->uniformLocation("morphing_param");

    m_cube_renderer.initialize(m_shader_program, m_pos_attr, m_col_attr, m_matrix_uniform, m_morphing_uniform);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        m_rotation_angle += 1.0f;
        update();
    });
    timer->start(10);
}


void Scene_widget::resizeGL(int w, int h)
{

}

void Scene_widget::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 rotation;
    rotation.rotate(m_rotation_angle, 1, 1, 1);

    m_cube_renderer.render(m_view_projection, rotation, m_rotations);
}

void Scene_widget::update_geometry()
{
    m_cube_renderer.setSplitParameter(m_split_parameter);
}


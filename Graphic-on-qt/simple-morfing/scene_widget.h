#ifndef SCENE_WIDGET_H
#define SCENE_WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include "cube_renderer.h"
class Scene_widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Scene_widget(QWidget *parent = nullptr);
    ~Scene_widget() = default;

    void set_split_parameter(size_t parameter);
    void set_morphing_parameter(float parameter);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void update_geometry();

    QMatrix4x4 m_view_projection;
    std::vector<QMatrix4x4> m_rotations;

    std::vector<GLfloat> m_vertices;
    std::vector<GLfloat> m_colors;
    std::vector<GLuint> m_indices;

    float m_rotation_angle { 0 };

    GLint m_pos_attr { 0 };
    GLint m_col_attr { 0 };
    GLint m_matrix_uniform { 0 };
    GLint m_morphing_uniform { 0 };
    QOpenGLShaderProgram *m_shader_program { nullptr };

    float m_morphing_parameter { 0 };
    size_t m_split_parameter { 10 };
    CubeRenderer m_cube_renderer;
};

#endif // SCENE_WIDGET_H

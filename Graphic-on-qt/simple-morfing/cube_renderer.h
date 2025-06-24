#ifndef CUBE_RENDERER_H
#define CUBE_RENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <vector>

class CubeRenderer : protected QOpenGLFunctions
{
public:
    CubeRenderer();
    ~CubeRenderer();

    void initialize(QOpenGLShaderProgram *shaderProgram, int posAttr, int colAttr, int matrixUniform, int morphingUniform);
    void setSplitParameter(size_t parameter);
    void setMorphingParameter(float parameter);
    void updateGeometry();
    void render(const QMatrix4x4 &viewProjection, const QMatrix4x4 &rotation, const std::vector<QMatrix4x4> &rotations);

private:
    size_t m_splitParameter = 1;
    float m_morphingParameter = 0.0f;

    std::vector<GLfloat> m_vertices;
    std::vector<GLfloat> m_colors;
    std::vector<GLuint> m_indices;

    QOpenGLShaderProgram *m_shaderProgram = nullptr;
    int m_posAttr = -1;
    int m_colAttr = -1;
    int m_matrixUniform = -1;
    int m_morphingUniform = -1;
};

#endif // CUBE_RENDERER_H

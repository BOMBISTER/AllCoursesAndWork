#include "cube_renderer.h"

CubeRenderer::CubeRenderer() {}

CubeRenderer::~CubeRenderer() {}

void CubeRenderer::initialize(QOpenGLShaderProgram *shaderProgram, int posAttr, int colAttr, int matrixUniform, int morphingUniform)
{
    initializeOpenGLFunctions();
    m_shaderProgram = shaderProgram;
    m_posAttr = posAttr;
    m_colAttr = colAttr;
    m_matrixUniform = matrixUniform;
    m_morphingUniform = morphingUniform;
}

void CubeRenderer::setSplitParameter(size_t parameter)
{
    m_splitParameter = parameter;
    updateGeometry();
}

void CubeRenderer::setMorphingParameter(float parameter)
{
    m_morphingParameter = parameter;
}

void CubeRenderer::updateGeometry()
{
    m_vertices.clear();
    m_colors.clear();
    m_indices.clear();

    for (size_t j = 0; j < m_splitParameter + 1; ++j) {
        for (size_t i = 0; i < m_splitParameter + 1; ++i) {
            m_vertices.push_back(-0.5f + GLfloat(i) / m_splitParameter);
            m_vertices.push_back(-0.5f + GLfloat(j) / m_splitParameter);
            m_vertices.push_back(-0.5f);

            m_colors.push_back(1.0f);
            m_colors.push_back(1.0f);
            m_colors.push_back(1.0f);
        }
    }

    for (size_t j = 0; j < m_splitParameter; ++j) {
        for (size_t i = 0; i < m_splitParameter; ++i) {
            m_indices.push_back(j * (m_splitParameter + 1) + i);
            m_indices.push_back(j * (m_splitParameter + 1) + i + 1);

            m_indices.push_back((j + 1) * (m_splitParameter + 1) + i);
            m_indices.push_back((j + 1) * (m_splitParameter + 1) + i + 1);

            m_indices.push_back(j * (m_splitParameter + 1) + i);
            m_indices.push_back((j + 1) * (m_splitParameter + 1) + i);

            m_indices.push_back(j * (m_splitParameter + 1) + i + 1);
            m_indices.push_back((j + 1) * (m_splitParameter + 1) + i + 1);
        }
    }
}

void CubeRenderer::render(const QMatrix4x4 &viewProjection, const QMatrix4x4 &rotation, const std::vector<QMatrix4x4> &rotations)
{
    m_shaderProgram->bind();

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, m_vertices.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, m_colors.data());

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    for (const auto &model : rotations) {
        m_shaderProgram->setUniformValue(m_matrixUniform, viewProjection * rotation * model);
        m_shaderProgram->setUniformValue(m_morphingUniform, m_morphingParameter);
        glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_shaderProgram->release();
    glDeleteBuffers(1, &EBO);
}

#include "graphicsobject.h"

#include <QColor>
#include <QDebug>

GraphicsObject::GraphicsObject()
    : m_positions(),
      m_colors(),
      m_vao(), m_positionVBO(), m_colorVBO(),
      m_positionOffset(0), m_colorOffset(0)
{
    initializeOpenGLFunctions();

    m_vao = new QOpenGLVertexArrayObject();
    m_positionVBO = new QOpenGLBuffer();
    m_colorVBO = new QOpenGLBuffer();

    initializeBuffers();
}

GraphicsObject::~GraphicsObject()
{
    m_positions.clear();
    m_colors.clear();

    delete m_vao;
    delete m_positionVBO;
    delete m_colorVBO;
}

void GraphicsObject::addPoint(QPointF position, QColor color)
{
    m_positions.push_back(position.x());
    m_positions.push_back(position.y());
    m_colors.push_back(color.red());
    m_colors.push_back(color.green());
    m_colors.push_back(color.blue());

    m_positionVBO->bind();
    m_positionVBO->allocate(m_positions.data(), m_positions.size() * sizeof(float));
    m_positionVBO->release();

    m_colorVBO->bind();
    m_colorVBO->allocate(m_colors.data(), m_colors.size() * sizeof(float));
    m_colorVBO->release();
//    m_vao->bind();

//    m_positionVBO->bind();

//    // Выделяем больше памяти
//    if (m_positionOffset + 2 * sizeof(float) > m_positionAllocated) {
//        m_positionVBO->allocate(m_positionAllocated * 2);
//        m_positionAllocated *= 2;
//        m_positionOffset = static_cast<int>(m_positions.size() * sizeof(float));

//    } else { // Добавляем вершину
//        m_positionVBO->write(m_positionOffset, m_positions.data(), 2 * sizeof(float));
//        m_positionOffset += 2 * sizeof(float);
//    }

//    m_positionVBO->release();

//    m_colorVBO->bind();

//    // Выделяем в два раза больше памяти
//    // если не хватило
//    if (m_colorOffset + 3 * sizeof(float) > m_colorAllocated) {
//        m_colorVBO->allocate(m_colorAllocated * 2);
//        m_colorVBO->write(0, m_colors.data(), m_colors.size() * sizeof(float));
//        m_colorAllocated *= 2;
//        m_colorOffset = static_cast<int>(m_colors.size() * sizeof(float));

//    } else { // иначе просто добавляем вершину
//        m_colorVBO->write(m_colorOffset, m_colors.data(), 2 * sizeof(float));
//        m_colorOffset += 3 * sizeof(float);
//    }

//    m_colorVBO->release();

//    m_vao->release();

    qDebug() << m_positions;
}

void GraphicsObject::clearPoints()
{
    m_positions.clear();
    m_colors.clear();
}

void GraphicsObject::render(int primitiveType)
{
    m_vao->bind();

    glDrawArrays(primitiveType, 0, m_positions.size() / 2);

    m_vao->release();
}

void GraphicsObject::initializeBuffers()
{
    m_vao->create();
    m_vao->bind();

    m_positionVBO->create();
    m_positionVBO->bind();
    m_positionVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);

    m_positionVBO->release();

    m_colorVBO->create();
    m_colorVBO->bind();
    m_colorVBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, 0);

    m_colorVBO->release();

    m_vao->release();
}

#include "vertex.h"

Vertex::Vertex()
    : m_x(0), m_y(0), m_z(0),
      m_color("black")
{

}

Vertex::Vertex(float x, float y, float z, const QColor &color)
    : m_x(x), m_y(y), m_z(z),
      m_color(color)
{

}

Vertex::Vertex(const QVector3D &point, const QColor &color)
    : m_x(point.x()),
      m_y(point.y()),
      m_z(point.z()),
      m_color(color)
{

}

QVector3D Vertex::getPosition() const
{
    return QVector3D(m_x, m_y, m_z);
}

void Vertex::setPosition(const QVector3D &position)
{
    m_x = position.x();
    m_y = position.y();
    m_z = position.z();
}

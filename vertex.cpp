#include "vertex.h"

Vertex::Vertex()
    : m_x(0), m_y(0), m_z(0)
{

}

Vertex::Vertex(float x, float y, float z)
    : m_x(x), m_y(y), m_z(z)
{

}

Vertex::Vertex(const QVector3D &point)
    : m_x(point.x()),
      m_y(point.y()),
      m_z(point.z())
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

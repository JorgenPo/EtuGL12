#include "splinevertex.h"

SplineVertex::SplineVertex()
{
    m_vectorLeft    = new QVector3D(0, 0, 0);
    m_vectorRight   = new QVector3D(0, 0, 0);
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_color = QColor(Qt::green);
}

SplineVertex::SplineVertex(Vertex &vertex)
{
    m_vectorLeft    = new QVector3D(0, 0, 0);
    m_vectorRight   = new QVector3D(0, 0, 0);
    this->setPosition(vertex.getPosition());
    m_color = vertex.getColor();
}

void SplineVertex::render()
{

}

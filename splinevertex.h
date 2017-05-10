#ifndef SPLINEVERTEX_H
#define SPLINEVERTEX_H

#include "vertex.h"

class SplineVertex : public Vertex
{
public:
    SplineVertex();
    SplineVertex(const Vertex& vertex);

    QVector3D* getVectorLeft() const { return m_vectorLeft; }
    QVector3D* getVectorRight() const { return m_vectorRight; }
    void setVectorLeft(QVector3D* vector) { m_vectorLeft = vector; }
    void setVectorRight(QVector3D* vector) { m_vectorRight = vector; }

    void render();

private:
    QVector3D*      m_vectorLeft;
    QVector3D*      m_vectorRight;

};

#endif // SPLINEVERTEX_H

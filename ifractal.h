#ifndef IFRACTAL_H
#define IFRACTAL_H

#include "vertex.h"
#include <vector>

class IFractalizer
{
public:
    virtual void fractalize(std::vector<Vertex> *vertices) = 0;

    virtual ~IFractalizer() = 0;
};

class LFractalizer : public IFractalizer
{
public:
    LFractalizer(float r, float alpha);

    ~LFractalizer();

    // IFractalizer interface
public:
    void fractalize(std::vector<Vertex> *vertices) override;

private:
    float m_r;
    float m_alpha;

    QVector3D m_smallVector;
    QVector3D m_bigVector;
    QVector3D m_bigVector2;
};

#endif // IFRACTAL_H

#include "ifractal.h"

#include <QtMath>
#include <QDebug>

LFractalizer::LFractalizer(float r = 0.05f, float alpha = 30.0f)
    : m_r(r), m_alpha(qDegreesToRadians(alpha)), m_smallVector(), m_bigVector()
{

}

LFractalizer::~LFractalizer()
{

}

void LFractalizer::fractalize(std::vector<Vertex> *vertices)
{
    static size_t iterations = 0;

    Vertex first;
    Vertex second;
    QVector3D diff;
    std::array<Vertex, 5> newVertices;
    float angle;
    float alpha;
    int sign = 1;
    auto iterator = vertices->begin();
    for (size_t i = 0; i + 1 < vertices->size(); i++) {
        first = vertices->at(i);
        second = vertices->at(i + 1);

        diff = second.getPosition() - first.getPosition();

        m_r = diff.length() / 2.8f;

        // Рассчитаем угол наклона вектора
        diff.normalize();
        angle = qAcos(QVector3D::dotProduct(diff, {1, 0, 0}));

        sign = 1;
        if ( first.getY() - second.getY() > 0 ) {
            alpha = m_alpha - angle;
        } else {
            alpha = m_alpha + angle;
        }

        m_smallVector = QVector3D(qCos(alpha), sign * qSin(alpha), 0.0f) * m_r;

        float bigR = m_r * qSin(qDegreesToRadians(108.0f)) /
                qSin(qDegreesToRadians(36.0f));

        float bigAlpha = qDegreesToRadians(36.0f) + alpha;
        m_bigVector = QVector3D(qCos(bigAlpha), qSin(bigAlpha), 0.0f) * bigR;
        float bigAlpha2 = qDegreesToRadians(36.0f) - alpha;
        m_bigVector2 = QVector3D(-qCos(bigAlpha2), qSin(bigAlpha2), 0.0f) * bigR;

        newVertices[0] = Vertex(first.getPosition() + m_smallVector, {first.getR(), second.getB(), 255});
        newVertices[1] = Vertex(first.getPosition() + m_bigVector, {first.getB(), second.getG(), first.getB()});
        newVertices[2] = Vertex(newVertices[1].getPosition() + m_smallVector, {255, first.getG(), second.getA()});
        newVertices[4] = Vertex(second.getPosition() - m_smallVector, {255, 15, second.getG() - first.getG()});
        newVertices[3] = Vertex(second.getPosition() + m_bigVector2, {second.getB(), second.getR(), second.getG()});
        // Вставляем пять элементов после текущего элемента
        iterator = vertices->begin() + i + 1;
        vertices->insert(iterator, newVertices.begin(), newVertices.end());
        i += 5;

//        qDebug() << "Iterations: " << ++iterations;
        ++iterations;
    }

    qDebug() << "TOTAL: " << iterations;
    iterations = 0;
}

IFractalizer::~IFractalizer()
{

}

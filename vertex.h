#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

#include <cstdint>
#include <array>

/**
 * Represents a vertex in 3D space.
 * This vertex has only position attribute.
 *
 * @brief The Vertex class
 * @author <popoff96@live.com> George Popoff
 * @date 27.02.2017
 */
class Vertex
{
public:
    Vertex();
    Vertex(float x, float y, float z = 0.0f);
    Vertex(const QVector3D &point);

    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getZ() const { return m_z; }
    QVector3D getPosition() const;

    void getX(float x) { m_x = x; }
    void getY(float y) { m_y = y; }
    void getZ(float z) { m_z = z; }
    void setPosition(const QVector3D &point);

private:
    float m_x, m_y, m_z;
};

#endif // VERTEX_H

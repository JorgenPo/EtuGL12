#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QColor>

#include <cstdint>
#include <array>

/**
 * Represents a vertex in 3D space.
 * This vertex has only position and color attribute.
 *
 * @brief The Vertex class
 * @author <popoff96@live.com> George Popoff
 * @date 28.02.2017
 */
class Vertex
{
public:
    Vertex();
    Vertex(float x, float y, float z = 0.0f, const QColor &color = QColor("black"));
    Vertex(const QVector3D &point, const QColor &color = QColor("black"));

    // Getters

    /* Position */
    float getX() const { return m_x; }
    float getY() const { return m_y; }
    float getZ() const { return m_z; }
    QVector3D getPosition() const;

    /* Color */
    float getR() const { return m_color.redF(); }
    float getG() const { return m_color.greenF(); }
    float getB() const { return m_color.blueF(); }
    float getA() const { return m_color.alphaF(); }
    const QColor& getColor() const { return m_color; }

    // Setters

    /* Position */
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }
    void setZ(float z) { m_z = z; }
    void setPosition(const QVector3D &point);

    /* Color */
    void setColor(const QColor &color) { m_color = color; }

private:
    float m_x, m_y, m_z;
    QColor m_color;
};

#endif // VERTEX_H

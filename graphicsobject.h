#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QPointF>
#include <QVector3D>
#include <vector>

/* Dynamic draw graphics object */
class GraphicsObject : private QOpenGLFunctions
{
    static const size_t VERTEX_START = 40;

public:
    GraphicsObject();

    ~GraphicsObject();

    void addPoint(QPointF position, QColor color);
    void render(int primitiveType = GL_LINE_STRIP);

private:
    void initializeBuffers();

private:
    std::vector<float> m_positions;
    std::vector<float> m_colors;

    QOpenGLVertexArrayObject* m_vao;
    QOpenGLBuffer* m_positionVBO;
    QOpenGLBuffer* m_colorVBO;

    int m_positionOffset;
    int m_colorOffset;
    int m_positionAllocated = VERTEX_START * sizeof(float);
    int m_colorAllocated = VERTEX_START * sizeof(float);
};

#endif // GRAPHICSOBJECT_H

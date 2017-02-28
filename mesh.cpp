#include "mesh.h"

#include <QDebug>

#include <cassert>

Mesh::Mesh(std::vector<Vertex> *vertices, QOpenGLBuffer::UsagePattern usage)
    : m_data(vertices)
{
    if ( !vertices ) {
        return;
    }

    m_buffer = std::make_unique<QOpenGLBuffer>();

    if ( !m_buffer->create() ) {
        qDebug() << "Cannot create QBuffer. Invalid context";
        throw NoOpenGLContextException();
    }

    if ( !m_buffer->bind() ) {
        qDebug() << "Cannot bind QBuffer to current context. "
                    "Probably different context used when "
                    "call() method invoked.";

        throw InvalidContextException();
    }

    m_buffer->setUsagePattern(usage);
}

/**
 * @brief Mesh::render
 *  Renders vertex by vertex all data or part of the data.
 * @param offset Offset in vertices
 * @param count  Count in vertices
 * @deprecated Non-shader function. Must be replaced!
 */
void Mesh::render(int offset, int count)
{
    if ( m_data ) {
        for ( const auto &vertex : *m_data ) {
            glColor4f(vertex.getR(), vertex.getG(),
                      vertex.getB(), vertex.getA());

            glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
        }
    }
}

void Mesh::setData(const std::vector<Vertex> &vertices)
{
    m_data = const_cast< std::vector<Vertex>* >(&vertices);
}

Mesh::~Mesh()
{
    if ( m_data ) {
        m_data->clear();
    }
    if ( m_buffer ) {
        m_buffer.release();
    }
}

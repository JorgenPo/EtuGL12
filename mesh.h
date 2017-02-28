#ifndef MESH_H
#define MESH_H

#include "vertex.h"

#include <vector>
#include <memory>

#include <QOpenGLBuffer>

class Mesh
{
public:
    Mesh(std::vector<Vertex> *vertices = nullptr,
         QOpenGLBuffer::UsagePattern usage = QOpenGLBuffer::StaticDraw);

    void render(int offset = 0, int count = -1);

    void setData(const std::vector<Vertex> &vertices);

    ~Mesh();
private:
    std::unique_ptr<QOpenGLBuffer>  m_buffer;
    std::vector<Vertex>            *m_data;
};

class NoOpenGLContextException : public std::exception
{
    const char* what() const throw()
    {
        return "There is no current QOpenGLContext found!";
    }
};

class InvalidContextException : public std::exception
{
    const char* what() const throw()
    {
        return "Current QOpenGLContext is invalid!";
    }
};


#endif // MESH_H

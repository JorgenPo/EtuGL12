#ifndef GLVIEW_H
#define GLVIEW_H

#include "mesh.h"

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>

class GLView : public QOpenGLWidget
{
    enum State {
        STATE_DRAW,
        STATE_SCISSORS,
        STATE_ERASE
    };

public:
    explicit GLView(QWidget *parent = nullptr);

    ~GLView();

public slots:
    void setPrimitiveType(int type);
    void clearVertices();

    void setColor(const QColor&);
    void setBackgroundColor(const QColor&);

    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    std::unique_ptr<Mesh> m_vMesh;
    std::unique_ptr<std::vector<Vertex>> m_vertices;

    int    m_primitiveType;

    QColor m_currentColor;
    QColor m_backgroundColor;

    State  m_state = State::STATE_DRAW;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // GLVIEW_H

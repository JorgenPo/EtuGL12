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
    void setDrawState();
    void setScissorState();
    void setEraseState();

    void setAlphaTestEnabled(bool enabled);
    void setBlendingEnabled(bool enabled);

    void setAlphaFunction(int function);
    void setAlphaRef(float ref);

    void setBlendingSfactor(int s);
    void setBlendingDfactor(int d);

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
    void stateChanged(State state);
    void disableStates();
    void scissorTest(bool enabled, float x = 0, float y = 0, float width = 100, float height = 100);

    bool   m_alphaTestEnabled = false;
    bool   m_blendingEnabled  = false;

    int    m_alphaFunc        = GL_ALWAYS;
    float  m_alphaRef         = 0.0f;

    int    m_blendingSfactor  = GL_ZERO;
    int    m_blendingDfactor  = GL_ZERO;

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

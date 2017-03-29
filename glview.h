#ifndef GLVIEW_H
#define GLVIEW_H

#include "ifractal.h"
#include "mesh.h"

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QRubberBand>

class GLView : public QOpenGLWidget
{
    Q_OBJECT

public:

    enum State {
        STATE_DRAW,
        STATE_SCISSORS,
        STATE_ERASE
    };

    explicit GLView(QWidget *parent = nullptr);

    virtual ~GLView();

    void setState(const State &state);

private:
    void disableStates();

signals:
    void scissorsRectChanged(const QRubberBand& rubberBand);

public slots:
    void setPrimitiveType(int type);
    void clearVertices();
    void fractalize();

    void setColor(const QColor&);
    void setBackgroundColor(const QColor&);

    void setAlphaTestEnabled(bool enabled);
    void setBlendingEnabled(bool enabled);
    void setScissorTestEnabled(bool enabled, int x = 0, int y = 0,
                               int width = 100, int height = 100);

    void setAlphaFunction(int function);
    void setAlphaRef(float ref);

    void setBlendingSfactor(int s);
    void setBlendingDfactor(int d);

    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    std::unique_ptr<Mesh> m_vMesh;
    std::unique_ptr<std::vector<Vertex>> m_vertices;

    int    m_primitiveType;

    QColor m_currentColor;
    QColor m_backgroundColor;

    State  m_state = State::STATE_DRAW;

    bool   m_alphaTestEnabled = false;
    bool   m_blendingEnabled  = false;

    int    m_alphaFunc        = GL_ALWAYS;
    float  m_alphaRef         = 0.0f;

    int    m_blendingSfactor  = GL_ZERO;
    int    m_blendingDfactor  = GL_ZERO;

    QRubberBand               *m_rubberBand;
    QPoint                    m_startPoint;
    int    m_scissorX         = 0;
    int    m_scissorY         = 0;
    int    m_scissorWidth     = 100;
    int    m_scissorHeight    = 100;

    IFractalizer* m_fractalizer;
};

#endif // GLVIEW_H

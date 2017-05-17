#ifndef APPLICATION_H
#define APPLICATION_H

#include "abstractscene.h"
#include "graphicsmesh.h"
#include "splinevertex.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class Application : public AbstractScene, QOpenGLFunctions
{
    enum State {
        STATE_NONE = -1,
        STATE_SPLINE = 4
    };

public:
    Application(QWidget* parent = nullptr);

    ~Application();

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    State m_state = State::STATE_NONE;

    QOpenGLShaderProgram* m_program;
    QOpenGLShader* m_vertexShader;
    QOpenGLShader* m_fragmentShader;

    std::vector<SplineVertex*> m_splineVertices;
    GraphicsObject* m_graphics;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // APPLICATION_H

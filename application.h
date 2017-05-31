#ifndef APPLICATION_H
#define APPLICATION_H

#include "abstractscene.h"
#include "graphicsobject.h"
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
        STATE_DRAW = 0,
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

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    bool isSelectedSplineVertex(const QVector3D &vertexPos,
                                const float &xCoord,
                                const float &yCoord,
                                const float &radiusX,
                                const float &radiusY);
    bool isSelectedSplineVector(const QVector3D &vectorPos,
                                const QVector3D &vertexPos,
                                const float &xCoord,
                                const float &yCoord,
                                const float &radiusX,
                                const float &radiusY);

private:
    State m_state = State::STATE_NONE;
    const float m_radius = 7.0f;

    QOpenGLShaderProgram* m_program;
    QOpenGLShader* m_vertexShader;
    QOpenGLShader* m_fragmentShader;

    std::vector<SplineVertex*> m_selectedVertices;
    std::vector<SplineVertex*> m_splineVertices;
    QVector3D* m_splineCurVector;

    GraphicsObject* m_graphics;
};

#endif // APPLICATION_H

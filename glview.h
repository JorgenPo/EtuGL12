#ifndef GLVIEW_H
#define GLVIEW_H

#include "abstractscene.h"
#include "ifractal.h"
#include "mesh.h"
#include "splinevertex.h"

#include <memory>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QRubberBand>

class GLView : public AbstractScene
{
    Q_OBJECT

public:

    enum State {
        STATE_NONE = -1,
        STATE_DRAW = 0,
        STATE_SCISSORS,
        STATE_ERASE,
        STATE_SELECT,
        STATE_SPLINE
    };

    enum Labs {
        LAB_1_2,
        LAB_3,
        LAB_4
    };

    explicit GLView(QWidget *parent = nullptr);

    virtual ~GLView();

    void setState(const State &state);

private:
    void disableStates();

signals:
    void scissorsRectChanged(const QRubberBand& rubberBand);
    void stateSplineSelected();

public slots:
    void clearVertices();
    void fractalize();
    void copyVertices();
    void pasteVertices();
    void rotateSelected(float angle);

    void setLab(GLView::Labs lab);
    void showSpline(GLView::State state);

    void setAlphaTestEnabled(bool enabled);
    void setBlendingEnabled(bool enabled);
    void setScissorTestEnabled(bool enabled, int x = 0, int y = 0,
                               int width = 100, int height = 100);

    void setAlphaFunction(int function);
    void setAlphaRef(float ref);

    void setBlendingSfactor(int s);
    void setBlendingDfactor(int d);

    //Spline mode
    void splineActivated(bool activated);

private slots:
    void labChanged();

    // QOpenGLWidget interface
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    // QWidget interface
public:
    void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);

public:
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

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event);

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    std::unique_ptr<Mesh> m_vMesh;
    std::unique_ptr<std::vector<Vertex>> m_vertices;
    std::vector<Vertex*> m_selectedVertices;
    std::vector<Vertex> m_copiedVertices;

    std::vector<SplineVertex*>      m_splineVertices;
    const size_t m_vertexRadius = 10.0f;
    QVector3D* m_splineCurVector;

    State  m_state = State::STATE_DRAW;
    Labs   m_lab   = Labs::LAB_1_2;

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

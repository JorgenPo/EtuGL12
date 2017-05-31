#include "application.h"
#include "utils.h"
#include <QMouseEvent>
#include <QSurfaceFormat>

Application::Application(QWidget* parent)
    : m_program(),
      m_vertexShader(),
      m_fragmentShader(),
      m_splineVertices()
{
    m_splineVertices.reserve(10);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    this->setFormat(format);
}

Application::~Application()
{
    delete m_program;
    delete m_vertexShader;
    delete m_fragmentShader;
}

void Application::initializeGL()
{
    initializeOpenGLFunctions();
    m_program = new QOpenGLShaderProgram(this);

    m_vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    m_vertexShader->compileSourceFile("shaders/simple.vert");

    m_fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    m_fragmentShader->compileSourceFile("shaders/simple.frag");

    m_program->addShader(m_vertexShader);
    m_program->addShader(m_fragmentShader);

    m_program->link();

    m_program->bind();

    m_program->bindAttributeLocation("position", 0);
    m_program->bindAttributeLocation("color", 1);

    m_graphics = new GraphicsObject();
}

void Application::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
}

void Application::paintGL()
{
    glClearColor(0.0f, 0.8f, 0.9f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    glEnable( GL_PROGRAM_POINT_SIZE );

    m_program->bind();
    m_program->setUniformValue("pointSize", m_radius);

    m_graphics->render(m_primitiveType);

    m_program->release();
}

void Application::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressed";
    float x = Utils::getInstanse().getAbsoluteX(event, this->size());
    float y = Utils::getInstanse().getAbsoluteY(event, this->size());
    float radiusX;
    float radiusY;
    radiusX = 2 * m_radius / static_cast<float>(this->size().width());
    radiusY = 2 * m_radius / static_cast<float>(this->size().height());

    if ( event->buttons() == Qt::LeftButton ) {
        // If pressed LeftButton -> add SplineVertex to selectedVertices
        for (SplineVertex* vertex : m_splineVertices) {
            if ( isSelectedSplineVertex(vertex->getPosition(),
                                        x, y, radiusX, radiusY)) {
                m_selectedVertices.clear();
                m_selectedVertices.push_back(vertex);
            }
        }
    } else if ( event->buttons() == Qt::RightButton ) {
        // If pressed RightButton -> add Vector to curVector
        for (SplineVertex* vertex : m_splineVertices) {
            // Add RightVector
            if ( isSelectedSplineVector(*vertex->getVectorRight(),
                                        vertex->getPosition(),
                                        x, y, radiusX, radiusY) ) {
                m_selectedVertices.clear();
                m_selectedVertices.push_back(vertex);
                m_splineCurVector = vertex->getVectorRight();
                qDebug() << m_splineCurVector << vertex->getVectorRight();
                break;
            }
            // Add LeftVector
            if ( isSelectedSplineVector(*vertex->getVectorLeft(),
                                        vertex->getPosition(),
                                        x, y, radiusX, radiusY) ) {
                m_selectedVertices.clear();
                m_selectedVertices.push_back(vertex);
                m_splineCurVector = vertex->getVectorLeft();
                qDebug() << m_splineCurVector << vertex->getVectorLeft();
                break;
            }
        }
    }
}

void Application::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouseMoved";
    if ( !m_selectedVertices.empty() ) {
        float x = Utils::getInstanse().getAbsoluteX(event, this->size());
        float y = Utils::getInstanse().getAbsoluteY(event, this->size());

        if ( event->buttons() == Qt::LeftButton ) {
            // If moved LeftButton -> move selectedVertices
            m_selectedVertices.front()->setX(x);
            m_selectedVertices.front()->setY(y);
        } else if ( event->buttons() == Qt::RightButton ) {
            // If moved RightButton -> move curVector
            *m_splineCurVector = QVector3D(x, y, 0) - m_selectedVertices.front()->getPosition();
        }
    }
}

void Application::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleased";
    if ( !m_selectedVertices.empty() ) {
        float x = Utils::getInstanse().getAbsoluteX(event, this->size());
        float y = Utils::getInstanse().getAbsoluteY(event, this->size());

        if ( event->buttons() == Qt::LeftButton ) {
            m_selectedVertices.front()->setX(x);
            m_selectedVertices.front()->setY(y);
        } else if ( event->buttons() == Qt::RightButton ) {
            *m_splineCurVector = QVector3D(x, y, 0) - m_selectedVertices.front()->getPosition();
        }
        m_selectedVertices.clear();
    }
}

void Application::mouseDoubleClickEvent(QMouseEvent *event)
{
    float x = Utils::getInstanse().getAbsoluteX(event, this->size());
    float y = Utils::getInstanse().getAbsoluteY(event, this->size());
    m_graphics->addPoint({x, y}, { m_currentColor.red(),
                                   m_currentColor.green(),
                                   m_currentColor.blue() });
}

bool Application::isSelectedSplineVertex(const QVector3D &vertexPos,
                                         const float &xCoord,
                                         const float &yCoord,
                                         const float &radiusX,
                                         const float &radiusY)
{
    float vertexX = vertexPos.x();
    float vertexY = vertexPos.y();
    if (vertexX - radiusX < xCoord &&
            vertexX + radiusX > xCoord &&
            vertexY - radiusY < yCoord &&
            vertexY + radiusY > yCoord) {
        return 1;
    }
    return 0;

}

bool Application::isSelectedSplineVector(const QVector3D &vectorPos,
                                         const QVector3D &vertexPos,
                                         const float &xCoord,
                                         const float &yCoord,
                                         const float &radiusX,
                                         const float &radiusY)
{
    float vectorX = vertexPos.x() + vectorPos.x();
    float vectorY = vertexPos.y() + vectorPos.y();
    if (vectorX - radiusX < xCoord &&
            vectorX + radiusX > xCoord &&
            vectorY - radiusY < yCoord &&
            vectorY + radiusY > yCoord) {
        return 1;
    }
    return 0;
}

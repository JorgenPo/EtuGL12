#include "glview.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QMatrix4x4>
#include <QtMath>

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent),
    m_vMesh(),
    m_vertices(), m_selectedVertices(), m_copiedVertices(),
    m_splineVertices(),
    m_primitiveType(1), m_currentColor(), m_backgroundColor(),
    m_startPoint(0, 0),
    m_lab(Labs::LAB_1_2),
    m_state(State::STATE_DRAW)
{
    m_vMesh = std::make_unique<Mesh>(nullptr, QOpenGLBuffer::DynamicDraw);

    m_vertices = std::make_unique< std::vector<Vertex> >();
    m_selectedVertices  = std::vector<Vertex*>();
    m_copiedVertices    = std::vector<Vertex>();
    m_splineVertices    = std::vector<SplineVertex*>();

    m_vMesh->setData(*m_vertices.get());

    QSurfaceFormat format;
    format.setVersion(1, 5);
    format.setSamples(16);

    this->setFormat(format);

    m_currentColor = Qt::black;
    m_backgroundColor = Qt::white;

    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_rubberBand->setPalette(QPalette(Qt::blue));
    m_rubberBand->setStyleSheet("background-color: #F5EEA7;");

    m_fractalizer = new LFractalizer(0.08f, 15.0f);
}

GLView::~GLView()
{
    if ( m_vMesh ) {
        m_vMesh.release();
    }

    if ( m_vertices ) {
        m_vertices.release();
    }

    if ( m_fractalizer ) {
        delete m_fractalizer;
    }
}

void GLView::setPrimitiveType(int type)
{
    m_primitiveType = type;
}

void GLView::clearVertices()
{
    m_vertices->clear();
    m_selectedVertices.clear();
}

void GLView::copyVertices()
{
    qDebug() << "Ctrl+C";
    if ( !m_selectedVertices.empty() ) {
        m_copiedVertices.clear();
    }
    for ( auto& vertex : m_selectedVertices ) {
        m_copiedVertices.push_back({ vertex->getPosition(), QColor(Qt::blue) });
    }
    for ( auto& vertex : m_copiedVertices ) {
        qDebug() << "COPY x = " << vertex.getX() << " y = " << vertex.getY();
    }
}

void GLView::pasteVertices()
{
    qDebug() << "Ctrl+V";
    m_selectedVertices.clear();
    Vertex* pVertex = nullptr;
    for ( auto& vertex : m_copiedVertices ) {
        vertex.setColor(QColor(Qt::green));
        m_vertices->push_back(vertex);
        pVertex = &(m_vertices->back());
        m_selectedVertices.push_back(pVertex);
        qDebug() << "PASTE x = " << vertex.getX() << " y = " << vertex.getY();
    }
}

void GLView::rotateSelected(float angle)
{
    QMatrix4x4 rotation;
    rotation.rotate(qDegreesToRadians(angle), 0, 0, 1);
    for ( auto* vertex : m_selectedVertices ) {
        vertex->setPosition((rotation * QVector4D(vertex->getPosition(), 0.0f)).toVector3D());
    }
}

void GLView::setLab(GLView::Labs lab)
{
    if (lab == m_lab) {
        return;
    }
    m_lab = lab;
    labChanged();
}

void GLView::showSpline(GLView::State state)
{
    qDebug() << "Show spline " << state;
    if ( state == State::STATE_SPLINE ) {
        qDebug() << "Spline " << state;
    }
}

void GLView::setColor(const QColor &color)
{
    if ( !color.isValid() ) {
        m_currentColor = Qt::black;
    } else {
        m_currentColor = color;
    }
}

void GLView::setBackgroundColor(const QColor &color)
{
    if ( !color.isValid() ) {
        m_backgroundColor = Qt::black;
    } else {
        m_backgroundColor = color;
    }
}

void GLView::setAlphaTestEnabled(bool enabled)
{
    m_alphaTestEnabled = enabled;
}

void GLView::setBlendingEnabled(bool enabled)
{
    m_blendingEnabled = enabled;
}

void GLView::setAlphaFunction(int function)
{
    m_alphaFunc = function;
}

void GLView::setAlphaRef(float ref)
{
    if ( ref < 0.0f || ref > 1.0f ) {
        return;
    }

    m_alphaRef = ref;
}

void GLView::setBlendingSfactor(int s)
{
    m_blendingSfactor = s;
    setBlendingEnabled(true);
}

void GLView::setBlendingDfactor(int d)
{
    m_blendingDfactor = d;
    setBlendingEnabled(true);
}

void GLView::splineActivated(bool activated)
{

}

void GLView::labChanged()
{
    switch ( m_lab ) {
    case Labs::LAB_1_2:
    case Labs::LAB_3:
        break;
    case Labs::LAB_4:
        //TODO Сделать однозначное соответствие между Vertex и SplineVertex,
        //чтобы без new SplineVertex(vertex); можно было обойтись.
        //и при смене лабы оставались вектора.
        m_splineVertices.clear();
        for (auto& vertex : *(m_vertices.get())) {
            SplineVertex* splineVertex = new SplineVertex(vertex);
            m_splineVertices.push_back(splineVertex);
            //            qDebug() << splineVertex << vertex.getX() << vertex.getY();
        }
        break;
    default:
        break;
    }
}

void GLView::initializeGL()
{

}

void GLView::resizeGL(int w, int h)
{

}

void GLView::paintGL()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(m_scissorX, m_scissorY, m_scissorWidth, m_scissorHeight);

    glClearColor(m_backgroundColor.redF(),
                 m_backgroundColor.greenF(),
                 m_backgroundColor.blueF(),
                 m_backgroundColor.alphaF());

    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glBegin(GL_POINTS);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
    }
    glEnd();

    if ( m_alphaTestEnabled ) {
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(m_alphaFunc, m_alphaRef);
    }

    if ( m_blendingEnabled ) {
        glEnable(GL_BLEND);
        glBlendFunc(m_blendingSfactor, m_blendingDfactor);
    }

    switch ( m_lab ) {
    case LAB_1_2:
    case LAB_3:
        glPointSize(2.0f);
        glLineWidth(1.0f);
        glBegin(m_primitiveType);
    {
        m_vMesh->render();
    }
        glEnd();
        break;
    case LAB_4:
        QVector3D* vector;
        float x, y, z;
        if ( !m_splineVertices.empty() ) {
            for ( const SplineVertex* vertex : m_splineVertices ) { //TODO Change to auto
                glPointSize( m_vertexRadius );
                glLineWidth(2.0f);
                //TODO Add functions for painting Vertex/Vector
                //Paint Vertex
                glBegin( GL_POINTS );
                {
                    glColor4f(vertex->getR(), vertex->getG(),
                              vertex->getB(), vertex->getA());
                    glVertex3f(vertex->getX(), vertex->getY(), vertex->getZ());
                }
                glEnd();

                //Paint Left vector
                vector = vertex->getVectorLeft();
                x = vertex->getX() + vector->x() + -0.1f;
                y = vertex->getY() + vector->y() + -0.1f;
                z = vertex->getZ() + vector->z();

                glBegin( GL_POINTS );
                {
                    glColor4f(0, 255, 0, 255);
                    glVertex3f(x, y, z);
                }
                glEnd();
                glBegin( GL_LINES );
                {
                    glColor4f(0, 255, 0, 255);
                    glVertex3f(vertex->getX(), vertex->getY(), vertex->getZ());
                    glColor4f(0, 255, 0, 255);
                    glVertex3f(x, y, z);
                }
                glEnd();

                //Paint Right vector
                vector = vertex->getVectorRight();
                x = vertex->getX() + vector->x() + 0.1f;
                y = vertex->getY() + vector->y() + 0.1f;
                z = vertex->getZ() + vector->z();

                glBegin( GL_POINTS );
                {
                    glColor4f(255, 0, 0, 255);
                    glVertex3f(x, y, z);
                }
                glEnd();
                glBegin( GL_LINES );
                {
                    glColor4f(255, 0, 0, 255);
                    glVertex3f(vertex->getX(), vertex->getY(), vertex->getZ());
                    glColor4f(255, 0, 0, 255);
                    glVertex3f(x, y, z);
                }
                glEnd();
            }
        }
        break;
    default:
        qDebug() << "Warning! Need add new LAB to GLView::Labs";
        break;
    }

    if ( !m_selectedVertices.empty() ) {
        for ( auto* vertex : m_selectedVertices) {
            glPointSize(3.0f);
            glBegin( GL_POINTS );
            {
                glColor3f(0.0f, 0.0f, 1.0f);
                glVertex2f(vertex->getX(), vertex->getY());
            }
            glEnd();
        }
    }

    if ( m_alphaTestEnabled ) {
        glDisable(GL_ALPHA_TEST);
    }

    if ( m_blendingEnabled ) {
        glDisable(GL_BLEND);
    }

    glDisable(GL_SCISSOR_TEST);
}

void GLView::setState(const State &state)
{
    m_state = state;
    qDebug() << "cur state = " << State(m_state);
    if ( state == STATE_ERASE ) {
        disableStates();
    }
}

void GLView::disableStates()
{
    qDebug() << "States disabled.";
    setScissorTestEnabled(false);
}

void GLView::fractalize()
{
    m_fractalizer->fractalize(m_vertices.get());
}

void GLView::setScissorTestEnabled(bool enabled, int x, int y, int width, int height)
{
    if ( enabled ) {
        m_scissorX = x;
        m_scissorY = y;
        m_scissorWidth = width;
        m_scissorHeight = height;
    } else {
        m_scissorX = 0;
        m_scissorY = 0;
        m_scissorWidth = this->size().width();
        m_scissorHeight = this->size().height();
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    float x = event->localPos().x();
    float y = event->localPos().y();

    x =  2 * x / static_cast<float>(this->size().width()) - 1.0f;
    y =  2 * -y / static_cast<float>(this->size().height()) + 1.0f;
    switch ( m_state ) {
    case STATE_DRAW:
        m_vertices->push_back({x, y, 0.0f, m_currentColor});
        break;
    case STATE_SCISSORS:
        m_startPoint = event->pos();
        m_rubberBand->setGeometry(QRect(m_startPoint, QSize()));
        m_rubberBand->show();
        break;
    case STATE_ERASE:
        break;
    case STATE_SELECT:
        m_startPoint = event->pos();
        m_rubberBand->setGeometry(QRect(m_startPoint, QSize()));
        m_rubberBand->show();
        break;
    case STATE_SPLINE:
        qDebug() << "STATE_SPLINE - Mouse Press";
        if (m_selectedVertices.empty()) {
            break;
        }
        qDebug() << "Selected items";
        foreach (Vertex* selectedVertex, m_selectedVertices) {
            qDebug() << selectedVertex->getX();
        }
        break;
    default:
        qDebug() << "Error mousePressEvent, m_state:" << m_state;
    }
}

void GLView::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key view";
    switch(event->key()) {
    case Qt::Key_F1:
        clearVertices();
        break;
    case Qt::Key_F2:
        fractalize();
        break;
    case Qt::Key_F3:
        rotateSelected(20.0f);
        break;
        //    case Qt::Key_C:
        //        qDebug() << "C";
        //        if ( event->modifiers() == Qt::CTRL ) {
        //            qDebug() << "Ctrl+C";
        //            for ( Vertex *vertex : *m_selectedVertices ) {
        //                m_copiedVertices->push_back({vertex->getX(), vertex->getY(), 0.0f, QColor(Qt::blue)});
        //                qDebug() << "Ctrl+C";
        //            }
        //        }
        //        break;
        //    case Qt::Key_V:
        //        if ( event->modifiers() == Qt::CTRL ) {
        //            for ( Vertex &vertex : *m_copiedVertices ) {
        //                m_vertices->push_back(nullptr, vertex);
        //                qDebug() << "Ctrl+V";
        //            }
        //        }
        //        break;
        //        for ( Vertex vertex : *m_copiedVertices ) {
        //            qDebug() << "COPY x = " << vertex.getX() << " y = " << vertex.getY();
        //        }
    default:
        break;
    }
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "State" << m_state;
    switch ( m_state ) {
    case STATE_DRAW:
        if ( event->buttons() == Qt::LeftButton) {
            this->mousePressEvent(event);
        }
        break;
    case STATE_SCISSORS:
        m_rubberBand->setGeometry(QRect(m_startPoint, event->pos()).normalized());
        break;
    case STATE_ERASE:
        break;
    case STATE_SELECT:
        m_rubberBand->setGeometry(QRect(m_startPoint, event->pos()).normalized());
        break;
    case STATE_SPLINE:
        break;
    default:
        break;
    }
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    float xLeft, xRight, yBottom, yTop;
    qDebug() << "State" << m_state;
    switch ( m_state ) {
    case STATE_DRAW:
        break;
    case STATE_SCISSORS:
        emit scissorsRectChanged(*m_rubberBand);

        m_rubberBand->hide();
        setScissorTestEnabled(true,
                              m_rubberBand->x(),
                              this->height() - m_rubberBand->y() - m_rubberBand->height(),
                              m_rubberBand->width(),
                              m_rubberBand->height());
        break;
    case STATE_ERASE:
        break;
    case STATE_SELECT:
        m_rubberBand->hide();
        m_selectedVertices.clear();

        xLeft    =  2 * m_rubberBand->geometry().left()     / static_cast<float>(this->size().width()) - 1.0f;
        xRight   =  2 * m_rubberBand->geometry().right()    / static_cast<float>(this->size().width()) - 1.0f;
        yBottom  =  2 * -m_rubberBand->geometry().bottom()  / static_cast<float>(this->size().height()) + 1.0f;
        yTop     =  2 * -m_rubberBand->geometry().top()     / static_cast<float>(this->size().height()) + 1.0f;

        for ( auto& vertex : *(m_vertices.get())) {
            if ( vertex.getX() > xLeft &&
                 vertex.getX() < xRight &&
                 vertex.getY() > yBottom &&
                 vertex.getY() < yTop) {
                m_selectedVertices.push_back(&vertex);
            }
        }
        break;
    default:
        break;
    }
}

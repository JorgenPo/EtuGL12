#include "glview.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent),
    m_vMesh(), m_vertices(), m_selectedVertices(),
    m_copiedVertices(),
    m_primitiveType(1), m_currentColor(), m_backgroundColor(),
    m_startPoint(0, 0)
{
    m_vMesh = std::make_unique<Mesh>(nullptr, QOpenGLBuffer::DynamicDraw);

    m_vertices          = std::make_unique< std::vector<Vertex> >(20);
    m_selectedVertices  = std::make_unique< std::vector<Vertex *> >(10);
    m_copiedVertices    = std::make_unique< std::vector<Vertex> >(10);
    m_vMesh->setData(*m_vertices.get());

    m_vertices->clear();
    m_selectedVertices->clear();
    m_copiedVertices->clear();

    QSurfaceFormat format;
    format.setVersion(1, 5);
    format.setSamples(16);

    this->setFormat(format);

    m_currentColor = Qt::black;
    m_backgroundColor = Qt::white;

    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_rubberBand->setPalette(QPalette(Qt::blue));
    m_rubberBand->setStyleSheet("background-color: #F5EEA7;");
}

GLView::~GLView()
{
    if ( m_vMesh ) {
        m_vMesh.release();
    }

    if ( m_vertices ) {
        m_vertices.release();
    }

    if ( m_selectedVertices ) {
        m_selectedVertices.release();
    }

    if ( m_copiedVertices ) {
        m_copiedVertices.release();
    }
}

void GLView::setPrimitiveType(int type)
{
    m_primitiveType = type;
}

void GLView::clearVertices()
{
    m_vertices->clear();
    m_selectedVertices->clear();
}

void GLView::copyVertices()
{
    qDebug() << "Ctrl+C";
    if ( !m_selectedVertices->empty() ) {
        m_copiedVertices->clear();
    }
    for ( Vertex *vertex : *m_selectedVertices ) {
        m_copiedVertices->push_back({vertex->getX(), vertex->getY(), 0.0f, QColor(Qt::blue)});
    }
    for ( Vertex vertex : *m_copiedVertices ) {
        qDebug() << "COPY x = " << vertex.getX() << " y = " << vertex.getY();
    }
}

void GLView::pasteVertices()
{
    qDebug() << "Ctrl+V";
    for ( Vertex &vertex : *m_copiedVertices ) {
        vertex.setColor(QColor(Qt::green));
        m_vertices->push_back(vertex);
        qDebug() << "PASTE x = " << vertex.getX() << " y = " << vertex.getY();
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

    glBegin(m_primitiveType);
    {
        m_vMesh->render();
    }
    glEnd();

    if ( !m_selectedVertices->empty() ) {
        for ( Vertex *vertex : *m_selectedVertices) {
            glPointSize(6.0f);
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
    if ( state == STATE_ERASE ) {
        disableStates();
    }
}

void GLView::disableStates()
{
    qDebug() << "States disabled.";
    setScissorTestEnabled(false);
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
    default:
        break;
    }
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    float xLeft, xRight, yBottom, yTop;
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
        m_selectedVertices->clear();

        xLeft    =  2 * m_rubberBand->geometry().left()     / static_cast<float>(this->size().width()) - 1.0f;
        xRight   =  2 * m_rubberBand->geometry().right()    / static_cast<float>(this->size().width()) - 1.0f;
        yBottom  =  2 * -m_rubberBand->geometry().bottom()  / static_cast<float>(this->size().height()) + 1.0f;
        yTop     =  2 * -m_rubberBand->geometry().top()     / static_cast<float>(this->size().height()) + 1.0f;

        for ( Vertex &vertex : *m_vertices ) {
            if ( vertex.getX() > xLeft &&
                 vertex.getX() < xRight &&
                 vertex.getY() > yBottom &&
                 vertex.getY() < yTop) {
                m_selectedVertices->push_back(&vertex);
            }
        }
        break;
    default:
        break;
    }
}

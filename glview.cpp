#include "glview.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent),
    m_vMesh(), m_vertices(), m_primitiveType(1),
    m_currentColor(), m_backgroundColor(), m_startPoint(0, 0)
{
    m_vMesh = std::make_unique<Mesh>(nullptr, QOpenGLBuffer::DynamicDraw);

    m_vertices = std::make_unique< std::vector<Vertex> >(20);
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
}

GLView::~GLView()
{
    if ( m_vMesh ) {
        m_vMesh.release();
    }

    if ( m_vertices ) {
        m_vertices.release();
    }
}

void GLView::setPrimitiveType(int type)
{
    m_primitiveType = type;
}

void GLView::clearVertices()
{
    m_vertices->clear();
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

    if ( m_blendingSfactor == GL_ZERO &&
         m_blendingDfactor == GL_ZERO ) {
        setBlendingEnabled(false);
    } else {
        setBlendingEnabled(true);
    }
}

void GLView::setBlendingDfactor(int d)
{
    m_blendingDfactor = d;

    if ( m_blendingSfactor == GL_ZERO &&
         m_blendingDfactor == GL_ZERO ) {
        setBlendingEnabled(false);
    } else {
        setBlendingEnabled(true);
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

    glBegin(m_primitiveType);
    {
        m_vMesh->render();
    }
    glEnd();

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
    default:
        qDebug() << "Error mousePressEvent, m_state:" << m_state;
    }
}

void GLView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_F1:
        m_vertices->clear();
        break;
    default:
        keyPressEvent(event);
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
    }
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    switch ( m_state ) {
    case STATE_DRAW:
        break;
    case STATE_SCISSORS:
        emit scissorsRectChanged(m_rubberBand->rect());

        m_rubberBand->hide();
        setScissorTestEnabled(true,
                              m_rubberBand->x(),
                              this->height() - m_rubberBand->y() - m_rubberBand->height(),
                              m_rubberBand->width(),
                              m_rubberBand->height());
        break;
    case STATE_ERASE:
        break;
    default:
        break;
    }
}

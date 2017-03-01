#include "glview.h"

#include <QColorDialog>
#include <QMouseEvent>
#include <QDebug>

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent),
    m_vMesh(), m_vertices(), m_primitiveType(1),
    m_currentColor(), m_backgroundColor()
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

void GLView::setDrawState()
{
    m_state = STATE_DRAW;
    disableStates();
    qDebug() << "DRAW" << "\n";
}

void GLView::setScissorState()
{
    m_state = STATE_SCISSORS;
    disableStates();
    qDebug() << "SCISSOR" << "\n";
}

void GLView::setEraseState()
{
    m_state = STATE_ERASE;
    disableStates();
}

void GLView::initializeGL()
{

}

void GLView::resizeGL(int w, int h)
{

}

void GLView::paintGL()
{
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
}

void GLView::disableStates()
{
    scissorTest(false);
}

void GLView::scissorTest(bool enabled, float x, float y, float width, float height)
{
    if ( enabled ) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    float x = event->localPos().x();
    float y = event->localPos().y();

    x =  2 * x / static_cast<float>(this->size().width()) - 1.0f;
    y =  2 * -y / static_cast<float>(this->size().height()) + 1.0f;

    m_vertices->push_back({x, y, 0.0f, m_currentColor});
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
    if ( event->buttons() == Qt::LeftButton) {
        this->mousePressEvent(event);
    }
}

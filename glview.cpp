#include "glview.h"

#include <QMouseEvent>

GLView::GLView(QWidget *parent) : QOpenGLWidget(parent),
    m_vMesh(), m_vertices(), m_primitiveType(1)
{
   m_vMesh = std::make_unique<Mesh>(nullptr, QOpenGLBuffer::DynamicDraw);


   m_vertices = std::make_unique< std::vector<Vertex> >(4);

   m_vertices->at(0) = {0.3f, 1.0f};
   m_vertices->at(1) = {0.0f, 0.4f};
   m_vertices->at(2) = {0.5f, 0.3f};
   m_vertices->at(3) = {0.4f, 0.1f};

   m_vMesh->setData(*(m_vertices.get()));

   QSurfaceFormat format;
   format.setVersion(1, 5);
   format.setSamples(16);

   this->setFormat(format);
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

void GLView::initializeGL()
{

}

void GLView::resizeGL(int w, int h)
{

}

void GLView::paintGL()
{
    glClearColor(0.3f, 1.0f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glBegin(GL_POINTS);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);
    }
    glEnd();

    glBegin(m_primitiveType);
    {
        m_vMesh->render();
    }
    glEnd();
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    float x = event->localPos().x();
    float y = event->localPos().y();

    x =  2 * x / static_cast<float>(this->size().width()) - 1.0f;
    y =  2 * -y / static_cast<float>(this->size().height()) + 1.0f;

    m_vertices->push_back({x, y});
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

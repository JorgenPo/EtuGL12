#include "application.h"
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
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    m_graphics->render(m_primitiveType);

    m_program->release();
}

void Application::mousePressEvent(QMouseEvent *event)
{
    float x = event->localPos().x();
    float y = event->localPos().y();

    x =  2 * x / static_cast<float>(this->size().width()) - 1.0f;
    y =  2 * -y / static_cast<float>(this->size().height()) + 1.0f;

    m_graphics->addPoint({x, y}, { m_currentColor.red(), m_currentColor.green(), m_currentColor.blue() });
}

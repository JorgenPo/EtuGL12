#include "abstractscene.h"

#include <QWidget>

AbstractScene::AbstractScene(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

AbstractScene::~AbstractScene()
{

}

void AbstractScene::setColor(const QColor &color)
{
    if ( !color.isValid() ) {
        m_currentColor = Qt::black;
    } else {
        m_currentColor = color;
    }
}

void AbstractScene::setBackgroundColor(const QColor &color)
{
    if ( !color.isValid() ) {
        m_backgroundColor = Qt::black;
    } else {
        m_backgroundColor = color;
    }
}

void AbstractScene::setPrimitiveType(int type)
{
    m_primitiveType = type;
}


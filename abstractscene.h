#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <QColor>
#include <QOpenGLWidget>


/**
 * @brief The AbstractScene class
 * Абстрактный класс для всех лабораторных работ.
 * Содержит реализацию основных функций.
 */
class AbstractScene : public QOpenGLWidget
{
    Q_OBJECT

public:
    AbstractScene(QWidget* parent = nullptr);
    virtual ~AbstractScene();

    virtual void setColor(const QColor&);
    virtual void setBackgroundColor(const QColor&);

public slots:
    virtual void setPrimitiveType(int type);

protected:
    QColor m_currentColor;
    QColor m_backgroundColor;

    int m_primitiveType;
};

#endif // ABSTRACTSCENE_H

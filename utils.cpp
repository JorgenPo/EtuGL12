#include "utils.h"

Utils::Utils()
{

}

float Utils::getAbsoluteX(QMouseEvent* event, QSize size) const
{
    float x = event->localPos().x();
    x = 2 * x / static_cast<float>(size.width()) - 1.0f;
    return x;
}

float Utils::getAbsoluteY(QMouseEvent* event, QSize size) const
{
    float y = event->localPos().y();
    y =  2 * -y / static_cast<float>(size.height()) + 1.0f;
    return y;
}

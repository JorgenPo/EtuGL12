#ifndef UTILS_H
#define UTILS_H

#include <QMouseEvent>



class Utils
{
private:
    static Utils instance;
    Utils();

public:
    static Utils& getInstanse() {
        static Utils instance;

        return instance;
    }

    float getAbsoluteX(QMouseEvent* event, QSize size) const;
    float getAbsoluteY(QMouseEvent* event, QSize size) const;
};

#endif // UTILS_H

#ifndef LIGHT_H
#define LIGHT_H
#include "rectangle.h"
class Light : public Rectangle
{
public:
    using Rectangle::Rectangle;
    void drawCube(QOpenGLShaderProgram &program);
};

#endif // LIGHT_H

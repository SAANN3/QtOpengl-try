#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QVector2D>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Rectangle : protected QOpenGLFunctions
{
public:
    Rectangle(float width,float height,float lenght,float transparency,float xPos,float yPos,float zPos);
    void drawCube(QOpenGLShaderProgram &program);
    QVector3D getPosVector();
protected:
    void initCube();
    float width;
    float height;
    float length;
    float transparency;
    float xPos;
    float yPos;
    float zPos;
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer verticesBuf;
    QOpenGLBuffer texcoordsBuf;
};

#endif // RECTANGLE_H

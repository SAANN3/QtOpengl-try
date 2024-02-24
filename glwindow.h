#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <rectangle.h>
#include <QList>
#include <QOpenGLExtraFunctions>
class GLWindow : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
    GLWindow();
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();



private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QMatrix4x4 viewMatrix;
    float mouseSpeed = 0.5;
    QVector3D cameraView;
    QVector3D cameraPos;
    QVector3D rotation;
    QPointF startingMousePos;
    QOpenGLShaderProgram program;
    QOpenGLShaderProgram programShadow;
    void initShaders();
    QMatrix4x4 m_projection;
    QList<Rectangle*> rectangles;
    float yaw = 0;
    float pitch = 0 ;
    uint depthMapFBO;
    uint depthMap;
    uint depthCubemap;
    int width;
    int height;

};


#endif // GLWINDOW_H

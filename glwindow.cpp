#include "glwindow.h"
#include <QVector2D>
#include <QVector3D>
#include <QSurface>
const int SHADOW_WIDTH = 4048;
const int SHADOW_HEIGHT = 4048;
float nearPlane = 0.1f;
float farPlane = 1555.0f;
QVector3D lightPos(-0.167291, 9.60021, -2.94585);
GLWindow::GLWindow()
{
    cameraPos = lightPos;
}

void GLWindow::initializeGL()
{

    initializeOpenGLFunctions();
    glClearColor(1.0f, 0.2f, 0.2f, 1.0f);

    //ground
    rectangles.append(new Rectangle(1000.0,0.5,1000.0,1.0,0.0,-10.0,0));
    ////rest
    rectangles.append(new Rectangle(1.0,10.5,10.0,1.0,-10.0,0.0,0));

    rectangles.append(new Rectangle(1.2,3.5,10.0,0.7,-5.5,-9,0));
    rectangles.append(new Rectangle(1.2,2.5,10.0,1,-2.9,-9,0));
    rectangles.append(new Rectangle(1.2,1.5,10.0,0.7,0.0,-9,0));
    rectangles.append(new Rectangle(1.0,1.0,1.0,1.0,0.0f,30.0f,-1.0f));
    rectangles.append(new Rectangle(10.5,0.5,0.5,1.0,-2.0,4.0f,-3.0f));
    rectangles.append(new Rectangle(20.5,20.5,0.5,1.0,-2.0,4.0f,-12.0f));
    rectangles.append(new Rectangle(1.5,10.5,0.5,1.0,4.0,4.0f,-11.0f));



    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




    // Create a texture for storing the depth map
    glGenFramebuffers(1, &depthMapFBO);


    //SECONDLIGHTTYPE
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    initShaders();
    update();
}

void GLWindow::resizeGL(int width, int height)
{
    this->width = width;
    this->height = height;
    float aspect = width/ (float)height;

    m_projection.setToIdentity();
    int angle = 90;

    m_projection.perspective(angle,aspect,0.1f,10000.0f);
    update();


}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);


    viewMatrix.setToIdentity();
    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(cameraPos, cameraPos+rotation,
                      QVector3D(0.0, 1.0, 0.0));

    program.bind();
    program.setUniformValue("qt_ModelViewProjectionMatrix", m_projection * viewMatrix);
    program.setUniformValue("viewPos",cameraPos);
    program.setUniformValue("lightPos",lightPos);
    program.setUniformValue("depthMap",depthMap);
    //transparency sorting based on camera position
    //Otherwise it will result to weird
    std::sort(rectangles.begin(), rectangles.end(), [this](Rectangle* r1, Rectangle* r2) {
        float distance1 = (cameraPos - r1->getPosVector()).length();
        float distance2 = (cameraPos - r2->getPosVector()).length();
        return (distance1 > distance2);
    });
    float aspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
    QMatrix4x4 shadowProj;
    QMatrix4x4 lookMatrix;
    shadowProj.perspective(90,aspect,nearPlane,farPlane);
    std::vector<QMatrix4x4> shadowTransforms;

    lookMatrix.lookAt(lightPos,lightPos + QVector3D(1.0,0.0,0.0),QVector3D(0.0,-1.0,0.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();
    lookMatrix.lookAt(lightPos,lightPos + QVector3D(-1.0,0.0,0.0),QVector3D(0.0,-1.0,0.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();
    lookMatrix.lookAt(lightPos,lightPos + QVector3D(0.0,1.0,0.0),QVector3D(0.0,0.0,1.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();
    lookMatrix.lookAt(lightPos,lightPos + QVector3D(0.0,-1.0,0.0),QVector3D(0.0,0.0,-1.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();
    lookMatrix.lookAt(lightPos,lightPos + QVector3D(0.0,0.0,1.0),QVector3D(0.0,-1.0,0.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();
    lookMatrix.lookAt(lightPos,lightPos + QVector3D(0.0,0.0,-1.0),QVector3D(0.0,-1.0,0.0));
    shadowTransforms.push_back(shadowProj * lookMatrix);
    lookMatrix = QMatrix4x4();


    programShadow.bind();
    programShadow.setUniformValue("far_plane",farPlane);
    programShadow.setUniformValue("lightPos",lightPos);
    for (int i = 0; i < 6; ++i){
        std::string name = "shadowMatrices[" + std::to_string(i) + "]";
        programShadow.setUniformValue(name.c_str(),shadowTransforms[i]);
    }

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

    for(int i = 0;i<rectangles.length();i++){
        rectangles.at(i)->drawCube(programShadow);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    programShadow.release();
    glCullFace(GL_BACK);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    program.setUniformValue("near_plane",nearPlane);
    program.setUniformValue("far_plane",farPlane);



    for(int i = 0;i<rectangles.length();i++){
        rectangles.at(i)->drawCube(program);
    }
    program.release();

}

void GLWindow::mousePressEvent(QMouseEvent *event)
{
    startingMousePos = event->globalPosition();
}
void GLWindow::mouseMoveEvent(QMouseEvent *event)
{
    //Get mouse position and compare it with previous value
    float deltaX = event->globalPosition().x() - startingMousePos.x();
    float deltaY = event->globalPosition().y() - startingMousePos.y();
    //Without pitch and yaw camera are rotating very weirdly
    yaw   += deltaX;
    pitch -= deltaY;
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    QVector3D direction;
    direction.setX(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.setY(sin(glm::radians(pitch)));
    direction.setZ(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    rotation = direction.normalized();
    startingMousePos = event->globalPosition();
    update();
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    //Movement with W A S D
    float step = 1.4f;
    if (event->key() == Qt::Key_W) {
        cameraPos += rotation * step;
    } else if (event->key() == Qt::Key_S) {
        cameraPos -= rotation * step;
    } else if (event->key() == Qt::Key_A) {
        cameraPos -= QVector3D::crossProduct(rotation,QVector3D(0,1,0)).normalized();
    } else if (event->key() == Qt::Key_D) {
        cameraPos += QVector3D::crossProduct(rotation,QVector3D(0,1,0)).normalized();
    } else if (event->key() == Qt::Key_E){
        lightPos.setX(lightPos.x() +0.5f);
    } else if (event->key() == Qt::Key_Q){
       lightPos.setX(lightPos.x() -0.5f);
    } else if (event->key() == Qt::Key_Z){
       lightPos.setY(lightPos.y() +0.5f);
    } else if (event->key() == Qt::Key_X){
        lightPos.setY(lightPos.y() -0.5f);
    }
    update();
}

void GLWindow::initShaders()
{
    //Initialize shaders, if something doesn't work we close the program
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.frag"))
        close();
    if (!programShadow.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/ShadowVShader.vsh"))
        close();
    if (!programShadow.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/ShadowFShader.frag"))
        close();
    if (!programShadow.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/ShadowDShader.gs"))
        close();
    if (!program.link())
        close();
    if (!programShadow.link())
        close();
}

#include "light.h"


void Light::drawCube(QOpenGLShaderProgram &program)
{
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 position = glm::mat4(1.0f);
    position = glm::translate(position,glm::vec3(xPos,yPos,zPos));
    indexBuf.bind();
    arrayBuf.bind();
    int offset = 0;

    int positionLocation = program.uniformLocation("position");
    program.setUniformValue(positionLocation,QMatrix4x4(glm::value_ptr(position)).transposed());

    int transparencyLocation = program.uniformLocation("transparency");
    program.setUniformValue(transparencyLocation,transparency);

    int vertexLocation = program.attributeLocation("qt_Vertex");
    program.enableAttributeArray(vertexLocation);
    program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    int aNormalLocation = program.attributeLocation("aNormal");
    program.enableAttributeArray(aNormalLocation);
    program.setAttributeBuffer(aNormalLocation, GL_FLOAT, offset, 3, sizeof(QVector3D));

    //program.setUniformValue("lightPos",QVector3D(10.0,0.0,0.0));

    glDrawElements(GL_TRIANGLE_STRIP,indexBuf.size(), GL_UNSIGNED_SHORT,0);

}

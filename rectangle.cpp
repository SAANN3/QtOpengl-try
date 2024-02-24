#include "rectangle.h"

Rectangle::Rectangle(float width,float height,float lenght,float transparency,float xPos,float yPos,float zPos)
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    this->width = width;
    this->height = height;
    this->length = lenght;
    this->xPos = xPos;
    this->yPos = yPos;
    this->zPos = zPos;
    this->transparency = transparency;
    initializeOpenGLFunctions();
    arrayBuf.create();
    indexBuf.create();
    verticesBuf.create();
    texcoordsBuf.create();
    initCube();
}
QVector3D Rectangle::getPosVector()
{
    return QVector3D(xPos,yPos,zPos);
}
void Rectangle::initCube()
{
    QVector3D vertices[] = {
        // Vertex data for face 0
        {QVector3D(-width, -height,  length)},  // v0
        {QVector3D( width, -height,  length)}, // v1
        {QVector3D(-width,  height,  length)},  // v2
        {QVector3D( width,  height,  length)}, // v3

        // Vertex data for face 1
        {QVector3D( width, -height,  length)}, // v4
        {QVector3D( width, -height, -length)}, // v5
        {QVector3D( width,  height,  length)},  // v6
        {QVector3D( width,  height, -length)}, // v7

        // Vertex data for face 2
        {QVector3D( width, -height, -length)}, // v8
        {QVector3D(-width, -height, -length)},  // v9
        {QVector3D( width,  height, -length)}, // v10
        {QVector3D(-width,  height, -length)},  // v11

        // Vertex data for face 3
        {QVector3D(-width, -height, -length)}, // v12
        {QVector3D(-width, -height,  length)},  // v13
        {QVector3D(-width,  height, -length)}, // v14
        {QVector3D(-width,  height,  length)},  // v15

        // Vertex data for face 4
        {QVector3D(-width, -height, -length)}, // v16
        {QVector3D( width, -height, -length)}, // v17
        {QVector3D(-width, -height,  length)}, // v18
        {QVector3D( width, -height,  length)}, // v19

        // Vertex data for face 5
        {QVector3D(-width,  height,  length)}, // v20
        {QVector3D( width,  height,  length)}, // v21
        {QVector3D(-width,  height, -length)}, // v22
        {QVector3D( width,  height, -length)}  // v23
    };
    QVector3D normals[] = {
        // Face 0
        QVector3D(0.0f, 0.0f, 1.0f),
        QVector3D(0.0f, 0.0f, 1.0f),
        QVector3D(0.0f, 0.0f, 1.0f),
        QVector3D(0.0f, 0.0f, 1.0f),

        // Face 1
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),
        QVector3D(1.0f, 0.0f, 0.0f),

        // Face 2
        QVector3D(0.0f, 0.0f, -1.0f),
        QVector3D(0.0f, 0.0f, -1.0f),
        QVector3D(0.0f, 0.0f, -1.0f),
        QVector3D(0.0f, 0.0f, -1.0f),

        // Face 3
        QVector3D(-1.0f, 0.0f, 0.0f),
        QVector3D(-1.0f, 0.0f, 0.0f),
        QVector3D(-1.0f, 0.0f, 0.0f),
        QVector3D(-1.0f, 0.0f, 0.0f),

        // Face 4
        QVector3D(0.0f, -1.0f, 0.0f),
        QVector3D(0.0f, -1.0f, 0.0f),
        QVector3D(0.0f, -1.0f, 0.0f),
        QVector3D(0.0f, -1.0f, 0.0f),

        // Face 5
        QVector3D(0.0f, 1.0f, 0.0f),
        QVector3D(0.0f, 1.0f, 0.0f),
        QVector3D(0.0f, 1.0f, 0.0f),
        QVector3D(0.0f, 1.0f, 0.0f)
    };

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };
    QVector2D texCoords[] = {
        // Texture coordinates for face 0
        {0.0, 0.0}, // v0
        {1.0, 0.0}, // v1
        {0.0, 1.0}, // v2
        {1.0, 1.0}, // v3

        // Texture coordinates for face 1
        {0.0, 0.0}, // v4
        {1.0, 0.0}, // v5
        {0.0, 1.0}, // v6
        {1.0, 1.0}, // v7

        // Texture coordinates for face 2
        {0.0, 0.0}, // v8
        {1.0, 0.0}, // v9
        {0.0, 1.0}, // v10
        {1.0, 1.0}, // v11

        // Texture coordinates for face 3
        {0.0, 0.0}, // v12
        {1.0, 0.0}, // v13
        {0.0, 1.0}, // v14
        {1.0, 1.0}, // v15

        // Texture coordinates for face 4
        {0.0, 0.0}, // v16
        {1.0, 0.0}, // v17
        {0.0, 1.0}, // v18
        {1.0, 1.0}, // v19

        // Texture coordinates for face 5
        {0.0, 0.0}, // v20
        {1.0, 0.0}, // v21
        {0.0, 1.0}, // v22
        {1.0, 1.0}  // v23
    };
    //! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(QVector3D));
    arrayBuf.release();
    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
    indexBuf.release();
    // Transfer normals data to VBO 2
    verticesBuf.bind();
    verticesBuf.allocate(normals,24 * sizeof(QVector3D));
    verticesBuf.release();
    // Transfer texture coordinates to VBO 3
    texcoordsBuf.bind();
    texcoordsBuf.allocate(texCoords,24 * sizeof(QVector2D));
    texcoordsBuf.release();
}

void Rectangle::drawCube(QOpenGLShaderProgram &program)
{
    //glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 position = glm::mat4(1.0f);
    position = glm::translate(position,glm::vec3(xPos,yPos,zPos));
    indexBuf.bind();
    arrayBuf.bind();


    int positionLocation = program.uniformLocation("position");
    program.setUniformValue(positionLocation,QMatrix4x4(glm::value_ptr(position)).transposed());

    int transparencyLocation = program.uniformLocation("transparency");
    program.setUniformValue(transparencyLocation,transparency);

    int vertexLocation = program.attributeLocation("qt_Vertex");
    program.enableAttributeArray(vertexLocation);
    program.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
    verticesBuf.bind();
    int aNormalLocation = program.attributeLocation("aNormal");
    program.enableAttributeArray(aNormalLocation);
    program.setAttributeBuffer(aNormalLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));
    texcoordsBuf.bind();
    int texCoordLocation = program.attributeLocation("aTexCoords");
    program.enableAttributeArray(texCoordLocation);
    program.setAttributeBuffer(texCoordLocation,GL_FLOAT,0,2,sizeof(QVector2D));






    glDrawElements(GL_TRIANGLE_STRIP,indexBuf.size(), GL_UNSIGNED_SHORT,0);

}

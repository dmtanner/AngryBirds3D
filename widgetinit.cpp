#include "widget.h"

void Widget::initializeShader()
{
    // To make a complete program, we must add all the stages
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../AngryBirds3D/BirdVertShader.vsh");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../AngryBirds3D/BirdFragShader.fsh");

    // Then we link the program (this part checks for compile errors)
    // This parts also checks that varyings match between the vertex and fragment stages
    program.link();

    // Binding the program makes it "active". When you call glDrawArrays,
    // it uses the "active" program for each vertex and fragment.
    // Since we only have one program, we can bind it in initialize().
    program.bind();

    // Next, we have to get a handle to the shader variables.
    // Since a shader program is on the graphics card, we get a number.
    // Commands say things like "set variable 3 to x"
    pvmMatrixUniform = program.uniformLocation("pvmMatrix");
    positionAttribute = program.attributeLocation("position");
    colorAttribute = program.attributeLocation("color");
    normAttribute = program.attributeLocation("normal");
}

void Widget::initializeBuffers()
{
    initializeSquare();
    initializeCircle();
    initializeCone();
    initializeCylinder();
    initializeSphere();
}


/*-----------------------------Drawing Functions-----------------------------------*/

void Widget::drawSquare()
{
    bindTriangleAttributes();
    program.setUniformValue(pvmMatrixUniform, projection * modelview);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void Widget::drawCircle()
{
    bindCircleAttributes();
    program.setUniformValue(pvmMatrixUniform, projection * modelview);
    glDrawArrays(GL_TRIANGLE_FAN, 0, SLICES + 2);
}

void Widget::drawCube()
{
    push();
        drawSquare();
        modelview.rotate( 90, 0, 1, 0);
        drawSquare();
        modelview.rotate( 90, 0, 1, 0);
        drawSquare();
        modelview.rotate( 90, 0, 1, 0);
        drawSquare();
        modelview.rotate( 90, 1, 0, 0);
        drawSquare();
        modelview.rotate( 180, 1, 0, 0);
        drawSquare();
    pop();
}

void Widget::drawCone()
{
    push();
        //draw top part of cone
        bindConeAttributes();
        program.setUniformValue(pvmMatrixUniform, projection * modelview);
        glDrawArrays(GL_TRIANGLE_FAN, 0, SLICES + 2);

        //draw bottom circle
        push();
        modelview.rotate(180, 1, 0, 0);
        drawCircle();
        pop();
    pop();
}

void Widget::drawCylinder()
{
    //draw sides of cylinder
    bindCylinderAttributes();
    program.setUniformValue(pvmMatrixUniform, projection * modelview);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, SLICES*2 + 2);

    //draw top and bottom circles
    push();
        modelview.translate(0, height/2, 0);
        drawCircle();
        modelview.translate(0, -height, 0);
        modelview.rotate(180, 1, 0, 0);
        drawCircle();
    pop();
}

void Widget::drawSphere()
{
    bindSphereAttributes();
    program.setUniformValue(pvmMatrixUniform, projection * modelview);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
}


/*-------------------------Bind Funtions--------------------------------------*/

void Widget::bindTriangleAttributes()
{
    // Now we put the position buffer on the GL_ARRAY_TARget
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    // glVertexAttribPointer lets us say to the GPU:
    // "Hey, you know attribute number positionAttribute? It pulls its data
    // "from the buffer on the GL_ARRAY_BUFFER target."
    // We must specify
    //   -which attribute pulls from the GL_ARRAY_BUFFER,
    //   -how many elements per attribute there are,
    //   -what kind of data each element is made of (how many bytes it takes)
    //   -should it use the original value, or should it normalize them
    //   -How many elements are there between attributes (usually none)
    //   -How many elements exist before the first attribute begins (usually none)
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Do the same thing for the color and normal buffers
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
    glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Widget::bindCircleAttributes()
{

    glBindBuffer(GL_ARRAY_BUFFER, circlePositionBuffer);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, circleColorBuffer);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, circleNormBuffer);
    glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Widget::bindConeAttributes()
{
    glBindBuffer(GL_ARRAY_BUFFER, conePositionBuffer);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, coneColorBuffer);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, coneNormBuffer);
    glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Widget::bindCylinderAttributes()
{
    glBindBuffer(GL_ARRAY_BUFFER, cylinderPositionBuffer);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderColorBuffer);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormBuffer);
    glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Widget::bindSphereAttributes()
{
    glBindBuffer(GL_ARRAY_BUFFER, spherePositionBuffer);
    glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer);
    glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //just use position buffer, since it's the same as the normal buffer in a sphere
    glBindBuffer(GL_ARRAY_BUFFER, sphereNormBuffer);
    glVertexAttribPointer(normAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer);
}


/*-------------------------Initialization Functions-----------------------------*/

void Widget::initializeSquare()
{
    //Generate Buffers
    GLuint bufferNames[3];
    glGenBuffers(3, bufferNames);
    colorBuffer = bufferNames[0];
    positionBuffer = bufferNames[1];
    normBuffer = bufferNames[2];

    //create and fill vectors with data
    std::vector<QVector3D> positionData;
    std::vector<QVector3D> colorData;
    std::vector<QVector3D> normData;

    positionData.push_back(QVector3D( -0.5, 0.5, -0.5));
    positionData.push_back(QVector3D( 0.5, 0.5, -0.5));
    positionData.push_back(QVector3D( 0.5, -0.5, -0.5));
    positionData.push_back(QVector3D( -0.5, -0.5, -0.5));

    colorData.push_back(blue);
    colorData.push_back(blue);
    colorData.push_back(blue);
    colorData.push_back(blue);

    normData.push_back(QVector3D(0, 0, -1));
    normData.push_back(QVector3D(0, 0, -1));
    normData.push_back(QVector3D(0, 0, -1));
    normData.push_back(QVector3D(0, 0, -1));


    // calculate the size (in bytes) of the data needed for the triangle
    GLuint posBufferSize = positionData.size() * sizeof(QVector3D);
    GLuint colorBufferSize = colorData.size() * sizeof(QVector3D);
    GLuint normBufferSize = normData.size() * sizeof(QVector3D);

    //bind and fill the position, color, and norm buffers
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, posBufferSize, &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, &colorData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normBuffer);
    glBufferData(GL_ARRAY_BUFFER, normBufferSize, &normData[0], GL_STATIC_DRAW);

}

void Widget::initializeCircle()
{
    GLuint bufferNames[3];
    glGenBuffers(3, bufferNames);
    circlePositionBuffer = bufferNames[0];
    circleColorBuffer = bufferNames[1];
    circleNormBuffer = bufferNames[2];

    std::vector<QVector3D> positionData;
    std::vector<QVector3D> colorData;
    std::vector<QVector3D> normData;

    //initial point
    positionData.push_back(QVector3D(0, 0, 0));
    colorData.push_back(red);
    normData.push_back(QVector3D(0.0, 1.0, 0.0));

    for(int i = 0; i <= SLICES; i++) {
        float theta = (i/(float)SLICES * 2.0 * PI);
        float x = cos(theta) * 0.5;
        float z = sin(theta) * 0.5;
        positionData.push_back(QVector3D(x, 0, z));
        colorData.push_back(red);
        normData.push_back(QVector3D(0.0, 1.0, 0.0));
    }

    GLuint bufferSize = positionData.size() * sizeof(QVector3D);
    GLuint colorBufferSize = colorData.size() * sizeof(QVector3D);
    GLuint normBufferSize = normData.size() * sizeof(QVector3D);

    glBindBuffer(GL_ARRAY_BUFFER, circlePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, circleColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, &colorData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, circleNormBuffer);
    glBufferData(GL_ARRAY_BUFFER, normBufferSize, &normData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

}

void Widget::initializeCone()
{
    GLuint bufferNames[3];
    glGenBuffers(3, bufferNames);
    conePositionBuffer = bufferNames[0];
    coneColorBuffer = bufferNames[1];
    coneNormBuffer = bufferNames[2];


    std::vector<QVector3D> positionData;
    std::vector<QVector3D> colorData;
    std::vector<QVector3D> normData;

    float yNorm = 1/sqrt(height*height + 1);

    positionData.push_back(QVector3D(0, height, 0));
    colorData.push_back(red);
    normData.push_back(QVector3D(0, yNorm, 0));

    //calculate vertices
    for(int i = 0; i <= SLICES; i++) {
        float theta = (i/(float)SLICES * 2.0 * PI);
        float x = cos(theta) * 0.5;
        float z = sin(theta) * 0.5;
        positionData.push_back(QVector3D(x, 0, z));
        colorData.push_back(red);
        normData.push_back(QVector3D(x, yNorm, z));
    }


    GLuint posBufferSize = positionData.size() * sizeof(QVector3D);
    GLuint colorBufferSize = colorData.size() * sizeof(QVector3D);
    GLuint normBufferSize = normData.size() * sizeof(QVector3D);

    glBindBuffer(GL_ARRAY_BUFFER, conePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, posBufferSize, &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, coneColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, &colorData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, coneNormBuffer);
    glBufferData(GL_ARRAY_BUFFER, normBufferSize, &normData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

}

void Widget::initializeCylinder()
{
    GLuint bufferNames[3];
    glGenBuffers(3, bufferNames);
    cylinderPositionBuffer = bufferNames[0];
    cylinderColorBuffer = bufferNames[1];
    cylinderNormBuffer = bufferNames[2];


    std::vector<QVector3D> positionData;
    std::vector<QVector3D> colorData;
    std::vector<QVector3D> normData;

    //calculate vertices
    for(int i = 0; i <= SLICES; i++) {
        float theta = (i/(float)SLICES * 2.0 * PI);
        float x = cos(theta) * 0.5;
        float z = sin(theta) * 0.5;
        positionData.push_back(QVector3D(x, height/2, z));
        positionData.push_back(QVector3D(x, -height/2, z));
        colorData.push_back(green);
        colorData.push_back(green);
        normData.push_back(QVector3D(x, 0, z));
        normData.push_back(QVector3D(x, 0, z));
    }


    GLuint posBufferSize = positionData.size() * sizeof(QVector3D);
    GLuint colorBufferSize = colorData.size() * sizeof(QVector3D);
    GLuint normBufferSize = normData.size() * sizeof(QVector3D);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, posBufferSize, &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, &colorData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderNormBuffer);
    glBufferData(GL_ARRAY_BUFFER, normBufferSize, &normData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

}

void Widget::initializeSphere()
{

    GLuint bufferNames[4];
    glGenBuffers(4, bufferNames);
    spherePositionBuffer = bufferNames[0];
    sphereColorBuffer = bufferNames[1];
    sphereNormBuffer = bufferNames[2];
    sphereIndexBuffer = bufferNames[3];

    std::vector<QVector3D> positionData;
    std::vector<QVector3D> colorData;
    std::vector<QVector3D> normData;
    std::vector<GLuint> indexData;


    //create vertices
    float radius = 0.5;

    for(int i = 0; i <= SLICES; i++) {
        float phi = (i/(float)SLICES * PI);
        float z = cos(phi) * radius;
        for(int j = 0; j <= SLICES; j++) {
            float theta = (j/(float)SLICES * 2.0 * PI);
            float x = sin(phi) * cos(theta) * radius;
            float y = sin(phi) * sin(theta) * radius;

            positionData.push_back(QVector3D(x, y, z));
            colorData.push_back(blue);
            normData.push_back(QVector3D(x, y, z));
        }
    }

    //create index buffer
    for(int i = 0; i < SLICES; i++) {
        for(int j = 0; j < SLICES; j++) {
            GLuint topLeft = (i * (SLICES + 1)) + j;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = topLeft + (SLICES + 1);
            GLuint bottomRight = bottomLeft + 1;

            indexData.push_back(topLeft);
            indexData.push_back(bottomLeft);
            indexData.push_back(topRight);
            indexData.push_back(bottomLeft);
            indexData.push_back(bottomRight);
            indexData.push_back(topRight);
        }
    }


    //calculate size of each buffer
    GLuint bufferSize = positionData.size() * sizeof(QVector3D);
    GLuint colorBufferSize = colorData.size() * sizeof(QVector3D);
    GLuint normBufferSize = normData.size() * sizeof(QVector3D);
    GLuint indexBufferSize = indexData.size() * sizeof(GLuint);

    indexSize = indexData.size();

    //bind and fill each buffer
    glBindBuffer(GL_ARRAY_BUFFER, spherePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, sphereColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorBufferSize, &colorData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, sphereNormBuffer);
    glBufferData(GL_ARRAY_BUFFER, normBufferSize, &normData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, &indexData[0], GL_STATIC_DRAW);

}


// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders(); //Fem el que hi ha a BL2GLWidget
    projLoc = glGetUniformLocation (program->programId(), "proj"); //Enviem al shader el uniform de la matriu de projecció
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective (float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f); //Creem matriu projecció amb els paràmetres adients
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
}

void MyGLWidget::paintGL() {
    BL2GLWidget::paintGL();  //Fem el que ja hi havia a la classe principal
    projectTransform(); //Afegim la matriu de projecció
}

MyGLWidget::~MyGLWidget() {
}



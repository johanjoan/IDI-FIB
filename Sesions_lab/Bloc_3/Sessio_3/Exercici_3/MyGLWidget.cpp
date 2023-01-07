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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_K : 
        pos_focus_escena.x -= 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_L :
        pos_focus_escena.x += 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_Left :
        pos_focus_escena.x -=0.1;
        pos_patricio.x -= 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_Right :
        pos_focus_escena.x +=0.1;
        pos_patricio.x += 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_Up :
        pos_focus_escena.z -=0.1;
        pos_patricio.z -= 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_Down :
        pos_focus_escena.z +=0.1;
        pos_patricio.z += 0.1;
        glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
        break;
    case Qt::Key_1 : //Apaguem/Encenem focus Escena
        if (col_focus_escena[0] == 0) col_focus_escena = glm::vec3(0.8,0.8,0.8);
        else col_focus_escena = glm::vec3(0,0,0);
        glUniform3fv (col_focus_escena_Loc, 1, &col_focus_escena[0]);
        break;
    case Qt::Key_2 : //Apaguem/Encenem focus Camera
        if (col_focus_camera[0] == 0) col_focus_camera = glm::vec3(0.8,0.8,0.8);
        else col_focus_camera = glm::vec3(0,0,0);
        glUniform3fv (col_focus_camera_Loc, 1, &col_focus_camera[0]);
        break;
    default: BL3GLWidget::keyPressEvent(event); break;
    }
    update();
}

void MyGLWidget::iniMaterialTerra () {
    amb = glm::vec3(0.2,0,0.2);
    diff = glm::vec3(0.0,0,0.8);
    spec = glm::vec3(1,1,1);
    shin = 100;
}


void MyGLWidget::initializeGL() {
    BL3GLWidget::initializeGL();
    pos_focus_Loc = glGetUniformLocation (program->programId(), "posFocus_escena");    
    col_focus_escena_Loc = glGetUniformLocation (program->programId(), "colorFocus_escena");    
    col_focus_camera_Loc = glGetUniformLocation (program->programId(), "colorFocus_camera");    
    pos_cam_Loc = glGetUniformLocation (program->programId(), "posFocus_camera");
    glUniform3fv (pos_cam_Loc, 1, &pos_focus_camera[0]);
    glUniform3fv (pos_focus_Loc, 1, &pos_focus_escena[0]);
    glUniform3fv (col_focus_escena_Loc, 1, &col_focus_escena[0]);
    glUniform3fv (col_focus_camera_Loc, 1, &col_focus_camera[0]);
}


void MyGLWidget::modelTransformPatricio ()
{
    TG = glm::translate(glm::mat4(1.f), pos_patricio);
    TG = glm::scale(TG, glm::vec3(0.3*escala/2,0.3*escala/2,0.3*escala/2));
    TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
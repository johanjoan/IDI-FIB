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


void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
    // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  if (!to_print) {
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }

  if (to_print) {
    // Pintem el cub-1
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, angle_cubs); //4 vegades més gran (costat 2.0)
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem el cub-2
    glBindVertexArray(VAO_Cub);
    modelTransformCub (5.0, angle_cubs + 2*M_PI/3.0); //5 vegades més gran (costat 2.0) i rotat un cert angle
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Pintem el cub-3
    glBindVertexArray(VAO_Cub);
    modelTransformCub (6.0, angle_cubs + 4*M_PI/3.0); //6 vegades més gran (costat 2.0) i rotat un cert angle
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG,angle,glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG,glm::vec3(5.0,0.0,0.0));
  TG = glm::scale(TG,glm::vec3(escala,escala,escala));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  // ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG,angle_pos_patricio,glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG,glm::vec3(5.0,0.0,0.0));
  TG = glm::rotate(TG,-float(M_PI/2.0),glm::vec3(0.0,1.0,0.0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::lookAt(glm::vec3(0, 2*radiEsc, 0),centreEsc,glm::vec3(1,0,0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc,zn,zf);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      to_print = !to_print;
    break;
	}
  case Qt::Key_1: {
    angle_pos_patricio = angle_cubs;
    emit num_cub(1);
    break;
	}
  case Qt::Key_2: {
    angle_pos_patricio = angle_cubs + 2*M_PI/3.0;
    emit num_cub(2);
    break;
	}
  case Qt::Key_3: {
    angle_pos_patricio = angle_cubs + 4*M_PI/3.0;
    emit num_cub(3);
    break;
	}
  case Qt::Key_F: {
      col_foc = !col_foc;
      if (col_foc) colFoc = glm::vec3(1.0,1.0,1.0); //Focus blanc
      else colFoc = glm::vec3(1.0,1.0,0.0); //Focus groc
      enviaColFocus();
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      viewTransform();
      projectTransform();
      emit canvi_camera1(camPlanta);
      emit canvi_camera2(!camPlanta);
    break;
	}
  case Qt::Key_Right: {
      angle_pos_patricio += 2*M_PI/3.0;
      angle_cubs += 2*M_PI/3.0;
    break;
	}
  case Qt::Key_Left: {
      angle_pos_patricio -=2*M_PI/3.0;
      angle_cubs -= 2*M_PI/3.0;
    break;
	}
  case Qt::Key_R: {
      angle_pos_patricio = 0;
      camPlanta = false;
      viewTransform();
      projectTransform();
      emit canvi_camera1(camPlanta);
      emit canvi_camera2(!camPlanta);
      colFoc = glm::vec3(1.0,1.0,1.0);
      enviaColFocus();
      emit num_cub(1);
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  // std::cout << angle_cubs << std::endl;
  // std::cout << angle_pos_patricio << std::endl;
  // std::cout << std::endl;
  update();
}

void MyGLWidget::set_camera(bool camera) {
  makeCurrent();
  camPlanta = !camera;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::set_cub_patricio(int cub) {
  makeCurrent();
  if (cub == 1) angle_pos_patricio = angle_cubs;
  else if(cub == 2) angle_pos_patricio = angle_cubs + 2*M_PI/3.0;
  else angle_pos_patricio = angle_cubs + 4*M_PI/3.0;
  update();
}


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
  case Qt::Key_A: {
    angle_morty += 45;
    if (angle_morty >= 360) angle_morty -= 360;      

    if (angle_morty > 180) morti_mira = true;
    else morti_mira = false;

    if (not morti_mira) {
      xFantasma += 0.5;
      if (xFantasma >= 4.5) restart();
    }
    break;
	}
  case Qt::Key_D: {
    angle_morty -= 45;
    if (angle_morty < 0) angle_morty += 360;
    if (angle_morty > 180) morti_mira = true;
    else morti_mira = false;

    if (not morti_mira) {
      xFantasma += 0.5;
      if (xFantasma >= 4.5) restart();
    }
    break;
	}
  case Qt::Key_M: {
    if (llanterna_encesa) color_llanterna = glm::vec3(0,0,0);
    else color_llanterna = glm::vec3(0.9,0.9,0.0);
    llanterna_encesa = !llanterna_encesa;
    glUniform3fv(colorllantLoc,1,&color_llanterna[0]);
    break;
	}
  case Qt::Key_E: {
      if (llum_escena) color_escena = glm::vec3(0,0,0);
      else color_escena = glm::vec3(0.7,0,0.7);
      llum_escena = !llum_escena;
      glUniform3fv(colorLoc,1,&color_escena[0]);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}



void MyGLWidget::initializeGL() {
  LL4GLWidget::initializeGL();

  //Uniforms per al FS
  colorLoc = glGetUniformLocation (program->programId(), "colorFocus");
  colorllantLoc = glGetUniformLocation (program->programId(), "colorFocusMorty");
  focusLoc = glGetUniformLocation(program->programId(), "posFocus");
  llumMortyLoc = glGetUniformLocation(program->programId(), "TG_llum");
  
  color_escena = glm::vec3(0.7,0,0.7); //Color dels focus d'escena
  color_llanterna = glm::vec3(0.9,0.9,0); //Color dels focus de la llanterna

  //Matriu amb la posició dels focus d'escena
  glm::mat4 posicio_focus(1.0f);
  posicio_focus[0] = glm::vec4(0,6.,0.,1.);
  posicio_focus[1] = glm::vec4(10.,6.,0.,1.);
  posicio_focus[2] = glm::vec4(0.,6.,10.,1.);
  posicio_focus[3] = glm::vec4(10.,6.,10.,1.);

  //Passem els Uniforms
  glUniformMatrix4fv (focusLoc, 1, GL_FALSE, &posicio_focus[0][0]);
  glUniform3fv(colorLoc,1,&color_escena[0]);
  glUniform3fv(colorllantLoc,1,&color_llanterna[0]);
  glUniform3fv(llumMortyLoc,1,&pos_llum_morty[0]); //Posició de la llanterna del Morty en SC Model

}

void MyGLWidget::modelTransformMorty() {
  TG_morty = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG_morty = glm::rotate(TG_morty, float(glm::radians(angle_morty)),glm::vec3(0.f, 1.f, 0.f));
  TG_morty = glm::scale(TG_morty, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG_morty = glm::translate(TG_morty, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG_morty[0][0]);
  glUniformMatrix4fv (llumMortyLoc, 1, GL_FALSE, &TG_morty[0][0]); //Passem la TG del morty per canviar de SCM a SCA la posició de la llanterna
}


void MyGLWidget::modelTransformFantasma () {
  glm::mat4 TG;
  if (not llum_escena and not morti_mira) {
    TG = glm::translate(glm::mat4(1.f), glm::vec3(xFantasma,0.5,5));
    TG = glm::rotate(TG, float(glm::radians(90.0)),glm::vec3(0.f, 1.f, 0.f));
    TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
    TG = glm::translate(TG, -centreBaseFantasma);
  }
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::restart() {
  llanterna_encesa = true;
  llum_escena = true;
  morti_mira = false;
  xFantasma = 0.5;
  color_llanterna = glm::vec3(0.9,0.9,0.0);
  color_escena = glm::vec3(0.7,0,0.7);
  angle_morty = 90.0;
  glUniform3fv(colorLoc,1,&color_escena[0]);
  glUniform3fv(colorllantLoc,1,&color_llanterna[0]);
}

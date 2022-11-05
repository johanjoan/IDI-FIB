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

void MyGLWidget::RickTransform() { //Col·loquem el Rick a la posició (-2.5,0,0) mirant cap a Z negativa
    LL2GLWidget::RickTransform();
    glm::mat4 TG(1.0f);  
    TG = glm::translate(TG, glm::vec3(-2.5, 0, 0));
    TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
    TG = glm::rotate(TG,float(M_PI), glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[RICK]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PortalTransform () { //Col·loquem el portal davant del Rick a una distancia de 3
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(-2.5,0,-3));
    TG = glm::scale(TG, glm::vec3(1.0/4,1,1)); //4 vegades més estret en X
    TG = glm::scale(TG, glm::vec3(escalaModels[PORTAL]));
    TG = glm::translate(TG, -centreBaseModels[PORTAL]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}



void MyGLWidget::iniCamera() {
    punt_max.x = 7.5; //Ho marca el terra
    punt_max.y = 3.0; //Ho marca alçada portal
    punt_max.z = 5.0; //Ho marca el terra
    punt_min.x = -7.5; //Terra  
    punt_min.y = 0.0;   //Terra i base personatges
    punt_min.z = -5.0;  //Terra
    centreEscena = glm::vec3((punt_max[0]+punt_min[0])/2.0,(punt_max[1]+punt_min[1])/2.0,(punt_max[2]+punt_min[2])/2.0);
    radiEscena = distance(punt_max,punt_min)/2;
    ra = 1.0f;
    dist = 2*radiEscena;
    obs = centreEscena + dist*glm::vec3(0,0,1); //Coloquem el observador a una distancia de 2*radi en l'eix Z
    vrp = centreEscena; //Mirem al centre de l'escena
    up = glm::vec3(0,1,0);
    fovini = 2.0 * asin(radiEscena / dist);
    fov = fovini;
    znear = dist - radiEscena;
    zfar = dist +radiEscena;
    std::cout << "IniCamera" << std::endl;
    MyGLWidget::viewTransform();
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj;
    Proj = glm::perspective (fov, ra, znear, zfar); //Creem matriu projecció amb els paràmetres adients
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
    std::cout << "PT" << std::endl;
}

void MyGLWidget::viewTransform () {
    glm::mat4 View (1.0f);
    View = glm::translate(View,glm::vec3(0.0,0.0,-dist)); //Allunyem l'escena una distancia d en l'eix z (la càmera està en el 0,0,0)
    View = glm::rotate(View,theta,glm::vec3(1.0,0.0,0.0)); //Rotem amunt i avall l'escena
    View = glm::rotate(View,psi,glm::vec3(0.0,1.0,0.0)); //Rotem cap als costats l'escena
    View = glm::translate(View,-vrp); //Col·loquem l'escena al centre (per a que ens caigui davant la càmera)
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::mouseMoveEvent (QMouseEvent *event) {
    makeCurrent();
    if (event->type()== QEvent::MouseMove and event->buttons() == Qt::LeftButton) { //Movem només si està el botó esquerra premut
        float x_new = event->x();
        float y_new = event->y();
        psi += (xClick-x_new)*factorAngleX;
        theta += (yClick-y_new)*factorAngleY;
        xClick = x_new;
        yClick = y_new;
        viewTransform();
        update();
    }
    update();
}
MyGLWidget::~MyGLWidget()
{

}


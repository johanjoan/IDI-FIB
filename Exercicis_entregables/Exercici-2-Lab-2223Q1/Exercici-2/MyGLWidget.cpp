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

// void MyGLWidget::initializeGL() {
//     LL2GLWidget::initializeGL();
//     glUniform1f (alternatiuLoc, alternatiu);
// }

void MyGLWidget::RickTransform() { //Col·loquem el Rick a la posició (-2.5,0,0) mirant cap a Z negativa
    // LL2GLWidget::RickTransform();
    alternatiu = 0;
    glm::mat4 TG(1.0f);  
    TG = glm::translate(TG, posicio_Rick);
    TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
    TG = glm::rotate(TG,angle_Rick, glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[RICK]);
    glUniform1f (alternatiuLoc, alternatiu);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PortalTransform () { //Col·loquem el portal davant del Rick a una distancia de 3
    alternatiu = 0;
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posicio_Portal);
    TG = glm::scale(TG, glm::vec3(1.0/4,1.0,1.0/4)); //4 vegades més estret
    TG = glm::scale(TG, glm::vec3(escalaModels[PORTAL]));
    TG = glm::rotate(TG,angle_Portal, glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[PORTAL]);
    glUniform1f (alternatiuLoc, alternatiu);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform () {
    alternatiu = 0;
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posMorty);
    TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
    TG = glm::rotate(TG,angleMorty, glm::vec3(0.0,1.0,0.0));
    TG = glm::translate(TG, -centreBaseModels[MORTY]);
    TGMorty = TG;
    glUniform1f (alternatiuLoc, alternatiu);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform2 () {
    alternatiu = 1;
    if (actualitzar_Morty_nou == 1) {
        TGMorty2 = TGMorty;
        actualitzar_Morty_nou = 0;
    }
    glUniform1f (alternatiuLoc, alternatiu);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TGMorty2[0][0]);
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
    MyGLWidget::viewTransform();
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj;
    Proj = glm::perspective (fov, ra, znear, zfar); //Creem matriu projecció amb els paràmetres adients
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
}

void MyGLWidget::viewTransform () {
    if (camera == 0) {
        glm::mat4 View (1.0f);
        View = glm::translate(View,glm::vec3(0.0,0.0,-dist)); //Allunyem l'escena una distancia d en l'eix z (la càmera està en el 0,0,0)
        View = glm::rotate(View,theta,glm::vec3(1.0,0.0,0.0)); //Rotem amunt i avall l'escena
        View = glm::rotate(View,psi,glm::vec3(0.0,1.0,0.0)); //Rotem cap als costats l'escena
        View = glm::translate(View,-vrp); //Col·loquem l'escena al centre (per a que ens caigui davant la càmera)
        glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
    else {
        glm::mat4 View (1.0f);
        View = glm::translate(View,glm::vec3(0.0,-0.8,0.0)); //Pugem la camera a alçada 0.8
        View = glm::rotate(View,-(angleMorty+float(M_PI)),glm::vec3(0.0,1.0,0.0)); //Rotem cap als costats l'escena
        View = glm::translate(View,-posMorty); //Col·loquem el Morty al centre
        glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
    }
}

void MyGLWidget::mouseMoveEvent (QMouseEvent *event) {
    makeCurrent();
    if (event->type()== QEvent::MouseMove and event->buttons() == Qt::LeftButton) { //Movem només si està el botó esquerra premut
        float x_new = event->x();
        float y_new = event->y();
        psi -= (xClick-x_new)*factorAngleX;
        theta -= (yClick-y_new)*factorAngleY;
        xClick = x_new;
        yClick = y_new;
        viewTransform();
        update();
    }
    update();
}

//Com que ja tenim guardada la posició del morty i la del portal com a variables de classe simplement mirem la distancia entre ells
//Aixo evita problemes al passar el portal, ja que sinó, el codi detecta que es passa 4 vegades el portal en comptes d'una
bool MyGLWidget::MortyPortalTest() { 
  return (glm::distance(posMorty,posicio_Portal) <= 0.2);
}


void MyGLWidget::Portal_passat() {
    posicio_Portal = glm::vec3 (-100,-100,-100); //Portal desapareix
    actualitzar_Morty_nou = 1;
    angle_Portal = float(M_PI/3.0); 
    
}

void MyGLWidget::carregaShaders() {
    LL2GLWidget::carregaShaders();
    alternatiuLoc = glGetUniformLocation (program->programId(),"alternatiu");
}

void MyGLWidget::creaBuffersModels () {
    LL2GLWidget::creaBuffersModels();
    nou_Morty.load("./models/Morty.obj");

    glGenVertexArrays(1, &VAO_nou_Morty);
    glBindVertexArray(VAO_nou_Morty);

    GLuint VBO_nou[2];
    glGenBuffers(2, VBO_nou);

    // geometria
    glBindBuffer(GL_ARRAY_BUFFER, VBO_nou[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nou_Morty.faces().size()*3*3,
        nou_Morty.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, VBO_nou[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*nou_Morty.faces().size()*3*3,
        nou_Morty.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);  
    glBindVertexArray (0);
}
  
void MyGLWidget::paintGL() {
    LL2GLWidget::paintGL();
    glBindVertexArray (VAO_nou_Morty);
    MyGLWidget::MortyTransform2();        
    glDrawArrays(GL_TRIANGLES, 0, nou_Morty.faces().size()*3);
    glBindVertexArray (0);
}

void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    // LL2GLWidget::keyPressEvent(event);
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_Q: { 
            angle_Rick = angle_Rick + M_PI/4.0;	
            break;
        }
        case Qt::Key_E: {
            angle_Rick = angle_Rick - M_PI/4.0;
            break;
        }
        case Qt::Key_P: {
            angle_Portal = angle_Rick;
            posicio_Portal_nova = glm::vec3(sin(angle_Rick),0.0,cos(angle_Rick))*glm::vec3(3) + posicio_Rick;
            if (posicio_Portal_nova == posicio_Portal) posicio_Portal = glm::vec3 (-100,-100,-100); //No es veurà el portal
            else posicio_Portal = posicio_Portal_nova;
            break;
        }
        case Qt::Key_Left: {
            angleMorty += M_PI/4.0;
            if (camera) viewTransform();
            break;
        }
        case Qt::Key_Right: {
            angleMorty -= M_PI/4.0;
            if (camera) viewTransform();
            break;
        }
        case Qt::Key_Up: { 
            posMorty = glm::vec3(sin(angleMorty),0.0,cos(angleMorty))*glm::vec3(0.1) + posMorty;
            if (MortyPortalTest()) Portal_passat();
            if (camera) viewTransform();
            break;
        }
        case Qt::Key_Down: {
            posMorty = posMorty - glm::vec3(sin(angleMorty),0.0,cos(angleMorty))*glm::vec3(0.1);  
            if (MortyPortalTest()) Portal_passat();	
            if (camera) viewTransform();
            break;
        }
        case Qt::Key_C: {
            if (!camera) {
                znear = 0.25;
                camera = 1;
                viewTransform();
                projectTransform();
                break;
            }
            znear = dist - radiEscena;
            camera = 0;
            viewTransform();
            projectTransform();
            break;
        }
        case Qt::Key_R: {
            camera = 0;
            psi = 0;
            theta = M_PI/4.0; 
            angle_Rick = M_PI;
            angle_Portal = M_PI;
            posicio_Portal = glm::vec3(sin(angle_Rick),0.0,cos(angle_Rick))*glm::vec3(3) + posicio_Rick; 
            posMorty = glm::vec3(1,0,0);
            angleMorty = 0;
            TGMorty2 = glm::mat4(0.0f);
            iniCamera();
            viewTransform();
            projectTransform();
            break;
        }
        default: event->ignore(); break;
    }
    update();
}

MyGLWidget::~MyGLWidget()
{

}


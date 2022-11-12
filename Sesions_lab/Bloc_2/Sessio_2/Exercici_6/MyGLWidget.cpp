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


void MyGLWidget::creaBuffers() {
    m.load ("models/Patricio.obj"); //Carreguem model Patricio


    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    //Accedim als VBOs
    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW); //Posició
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    GLuint VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);	


    //Creem el terra
    glm::vec3 posicio[6] = {
        glm::vec3(-2.5,0,2.5),
        glm::vec3(-2.5,0,-2.5),
        glm::vec3(2.5,0,-2.5),
        glm::vec3(-2.5,0,2.5),
        glm::vec3(2.5,0,-2.5),
        glm::vec3(2.5,0,2.5),
    };
    glm::vec3 colorTerra[6] = {
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
    };


    // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAOTerra);
    glBindVertexArray(VAOTerra);

    GLuint VBOTerra[2];
    glGenBuffers(2, VBOTerra);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(posicio), posicio, GL_STATIC_DRAW);

    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBOTerra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorTerra), colorTerra, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    //Fi
    glBindVertexArray (0);
}

void MyGLWidget::paintGL() {
    

    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    modelTransformPatricio();

    // Activem el VAO per a pintar el Patricio
    glBindVertexArray (VAO1);

    // pintem Patricio
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    //Transformació Terra
    modelTransformTerra();

    //Pintem Terra
    glBindVertexArray(VAOTerra);

    glDrawArrays(GL_TRIANGLES,0,6);

    glBindVertexArray (0);
}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL(); 
    glEnable(GL_DEPTH_TEST); //Z-buffer
    ini_camera();
}


void MyGLWidget::viewTransform() {

    glm::mat4 View = glm::lookAt (obs,vrp,up);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::ini_camera() {
    MyGLWidget::calcula_escena();
    ra = 1.0f;
    float dist = 2*radi;
    obs = centre_escena + dist*glm::vec3(0,0,1); //Coloquem el observador a una distancia de 2*radi en l'eix Z
    vrp = centre_escena; //Mirem al centre de l'escena
    up = glm::vec3(0,1,0);
    FOVini = 2.0 * asin(radi / dist);
    FOV = FOVini;
    znear = dist - radi;
    zfar = dist +radi;
    //Iniciem PM i VM
    projectTransform();
    viewTransform();

}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation (program->programId(), "proj"); //Enviem al shader el uniform de la matriu de projecció
    viewLoc = glGetUniformLocation (program->programId(), "view");
}


void MyGLWidget::keyPressEvent (QKeyEvent *event) {
    BL2GLWidget::keyPressEvent (event);
    switch (event->key()) {
        case Qt::Key_R: { // escalar a més gran
        rotation += M_PI/4.0f;
        break;
        }
        case Qt::Key_O: {
            if (opt == 1) opt = 0;
            else opt = 1;
            projectTransform();
            break;
        }
        default: event->ignore(); break;
    }
}


void MyGLWidget::modelTransformPatricio() {
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    transform = glm::rotate(transform,rotation,glm::vec3(0.0,1.0,0.0));
    transform = glm::scale(transform,glm::vec3(escala_patricio)); //Escalem per a tindre alçada 4
    transform = glm::translate(transform,-base_patricio); //Col·loquem el patricio al (0,0,0)
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}
void MyGLWidget::modelTransformTerra() {
    glm::mat4 transform (1.0f);
    transform = glm::scale(transform, glm::vec3(escala));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);

}

void MyGLWidget::parametres_escena(glm::vec3 punt_max,glm::vec3 punt_min) {
    centre_escena = glm::vec3((punt_max[0]+punt_min[0])/2.0,(punt_max[1]+punt_min[1])/2.0,(punt_max[2]+punt_min[2])/2.0);
    radi = distance(punt_max,punt_min)/2;
    radi_ample = radi;
    radi_alt = radi;
}


void MyGLWidget::resizeGL(int w, int h){
    float rav = float(w)/float(h);
    if (opt) {
        ra = rav;//Cas en el que rav > 1
        if (rav < 1) { //Cas en el que rav < 1
            FOV = 2*atan(tan(FOVini/2.0)/rav);
        }
    }
    else {
        if (rav > 1) radi_ample = radi*rav;
        else radi_alt = radi/rav;
    }
    projectTransform (); //Tornem a canviar la projecció per aplicar el nou FOV i ra del window.
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj;
    if (opt == 1) {
        Proj = glm::perspective (FOV, ra, znear, zfar); //Creem matriu projecció amb els paràmetres adients
    }
    else {
        Proj = glm::ortho(-radi_ample,radi_ample,-radi_alt,radi_alt,znear,zfar);
    }

    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
}

void MyGLWidget::calcula_escena() { //Calcula els punts maxims i minims de l'escena i la base del patricio
    float minx,miny,minz,maxx,maxy,maxz;
    minx = maxx = m.vertices()[0];
    miny = maxy = m.vertices()[1];
    minz = maxz = m.vertices()[2];
    for (unsigned int i = 3; i < m.vertices().size(); i+=3) {
        if (m.vertices()[i] < minx) minx = m.vertices()[i];
        if (m.vertices()[i] > maxx) maxx = m.vertices()[i];
        if (m.vertices()[i+1] < miny) miny = m.vertices()[i+1];
        if (m.vertices()[i+1] > maxy) maxy = m.vertices()[i+1];
        if (m.vertices()[i+2] < minz) minz = m.vertices()[i+2];
        if (m.vertices()[i+2] > maxz) maxz = m.vertices()[i+2];
    }

    base_patricio = glm::vec3((minx+maxx)/2.0,miny,(minz+maxz)/2.0); 
    escala_patricio = 4.0/(maxy-miny); //Volem altura 4

    punt_max.x = 2.5; //Ho marca el terra
    punt_max.y = 4.0; //Ho marca alçada patricio
    punt_max.z = 2.5; //Ho marca el terra
    punt_min.x = -2.5; //Terra  
    punt_min.y = 0.0;   //Patricio
    punt_min.z = -2.5;  //Terra
    parametres_escena(punt_max,punt_min); //Calculem els parametres de l'escena a partir de tindre el punt minim i maxim

}



MyGLWidget::~MyGLWidget() {
}


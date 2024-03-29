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

    //PRIMER PATRICIO

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
    // Activem el VAO per a pintar els Patricios
    glBindVertexArray (VAO1);

    //PRIMER PATRICIO
    // rotation = rotation - M_PI; //Rotació primer Patricio (mirant cap a la +Z)
    posicio_patricio = glm::vec3(2,0,2); //Posició del primer Patricio
    // Carreguem la transformació de model
    modelTransformPatricio();


    // pintem Patricio
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    //SEGON PATRICIO

    posicio_patricio = glm::vec3(0,0,0); //Posició del primer Patricio
    rotation = rotation + M_PI/2.0; //Rotació segon Patricio (Mirant cap a +X)
    // Carreguem la transformació de model
    modelTransformPatricio();
    // pintem Patricio
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    //TERCER PATRICIO
    rotation = rotation + M_PI/2.0; //Rotació tercer Patricio (Mirant cap a -Z)
    posicio_patricio = glm::vec3(-2,0,-2); //Posició del primer Patricio
    // Carreguem la transformació de model
    modelTransformPatricio();
    // pintem Patricio
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    rotation = rotation - M_PI; //Tornem a la rotacio "original" per a quan tornem a pintar el primer Patricio

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

void MyGLWidget::projectTransform() {
    glm::mat4 Proj;
    if (opt == 1) {
        Proj = glm::perspective (FOV, ra, znear, zfar); //Creem matriu projecció amb els paràmetres adients
    }
    else {
        Proj = glm::ortho(left,right,bottom,top,znear,zfar);
    }

    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
}

void MyGLWidget::viewTransform() {
    glm::mat4 View (1.0f);
    View = glm::translate(View,glm::vec3(0.0,0.0,-dist)); //Allunyem l'escena una distancia d en l'eix z (la càmera està en el 0,0,0)
    View = glm::rotate(View,theta,glm::vec3(1.0,0.0,0.0)); //Rotem amunt i avall l'escena
    View = glm::rotate(View,phi,glm::vec3(0.0,1.0,0.0)); //Rotem cap als costats l'escena
    View = glm::translate(View,-vrp); //Col·loquem l'escena al centre (per a que ens caigui davant la càmera)
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::ini_camera() {
    MyGLWidget::calcula_escena();
    ra = 1.0f;
    dist = 2*radi;
    obs = centre_escena + dist*glm::vec3(0,0,1); //Coloquem el observador a una distancia de 2*radi en l'eix Z
    vrp = centre_escena; //Mirem al centre de l'escena
    up = glm::vec3(0,1,0);
    FOVini = 2.0 * asin(radi / dist);
    FOV = FOVini;
    emit envia_FOV(FOV*180/M_PI);
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
        case Qt::Key_Z:{
            FOV -= 0.1; //Reduim FOV (Zoom in) 
            //Parametres projecció ortogonal
            left += 0.2;
            right -= 0.2;
            top -= 0.2;
            bottom += 0.2;
            emit envia_FOV(FOV*180/M_PI);
            projectTransform(); //Actualitzem la perspectiva
            break;
        }
        case Qt::Key_X:{
            FOV += 0.1; //Augmentem FOV (zoom out)
            //Paràmetres projecció ortogonal
            left -= 0.2;
            right += 0.2;
            top += 0.2;
            bottom -= 0.2;
            emit envia_FOV(FOV*180/M_PI);
            projectTransform(); //Actualitzem la perspectiva
            break;
        }
        default: event->ignore(); break;
    }
}


void MyGLWidget::modelTransformPatricio() {
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform,posicio_patricio); //El movem a la posició que toqui (Cada patricio a una posició diferent)
    transform = glm::rotate(transform,rotation,glm::vec3(0.0,1.0,0.0)); //Rotem el Patricio
    transform = glm::scale(transform,glm::vec3(escala_patricio)); //Escalem per a tindre alçada 1
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
    left = -radi;
    right = radi;
    top = radi;
    bottom = -radi;

}


void MyGLWidget::resizeGL(int w, int h){
    float rav = float(w)/float(h);
    ra = rav;//Cas en el que rav > 1
    if (rav < 1) { //Cas en el que rav < 1
        FOV = 2*atan(tan(FOVini/2.0)/rav);
        emit envia_FOV(FOV*180/M_PI);
    }
    projectTransform (); //Tornem a canviar la projecció per aplicar el nou FOV i ra del window.
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
    escala_patricio = 1.0/(maxy-miny); //Volem altura 1

    punt_max.x = 2.5; //Ho marca el terra
    punt_max.y = 1.0; //Ho marca alçada patricio
    punt_max.z = 2.5; //Ho marca el terra
    punt_min.x = -2.5; //Terra  
    punt_min.y = 0.0;   //Patricio
    punt_min.z = -2.5;  //Terra
    parametres_escena(punt_max,punt_min); //Calculem els parametres de l'escena a partir de tindre el punt minim i maxim

}

void MyGLWidget:: mouseMoveEvent(QMouseEvent *event) {
    makeCurrent();
    if (event->type()== QEvent::MouseMove and event->buttons() == Qt::LeftButton) { //Movem només si està el botó esquerra premut
        x_new = event->x();
        y_new = event->y();
        phi += (x_old-x_new)*M_PI/500;
        theta += (y_old-y_new)*M_PI/500;
        x_old = x_new;
        y_old = y_new;
        viewTransform();
        update();
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    makeCurrent();
    if (event->buttons() == Qt::LeftButton) {
        x_old = event->x();
        y_old = event->y();
    }
}


void MyGLWidget::set_FOV(int valor) {
    makeCurrent();
    FOV = valor*M_PI/180.0;
    projectTransform();
    update();
}

MyGLWidget::~MyGLWidget() {
}



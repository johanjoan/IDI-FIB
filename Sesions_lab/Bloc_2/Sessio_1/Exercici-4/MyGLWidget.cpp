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
    m.load ("models/HomerProves.obj"); //Carreguem model Homer


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

    //Fi
    glBindVertexArray (0);
}

void MyGLWidget::paintGL() {
    

    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    modelTransform ();

    // Activem el VAO per a pintar el homer
    glBindVertexArray (VAO1);

    // pintem
    glDrawArrays (GL_TRIANGLES, 0, m.faces ().size () * 3);

    glBindVertexArray (0);
}

void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL(); 
    glEnable(GL_DEPTH_TEST); //Z-buffer
    ini_camera();
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective (FOV, ra, znear, zfar); //Creem matriu projecció amb els paràmetres adients
    glUniformMatrix4fv(projLoc,1,GL_FALSE,&Proj[0][0]); //Assignar al uniform
}

void MyGLWidget::viewTransform() {
    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (obs,vrp,up);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::ini_camera() {
    //Inicialització PM
    FOV = float(M_PI)/1.6f;
    ra = 1.0f;
    znear = 0.4f;
    zfar = 3.0f;
    //Inicialització VM
    obs = glm::vec3(0,0.0,1.0);
    vrp = glm::vec3(0,0,0);
    up = glm::vec3(0,1,0);

    //Iniciem PM i VM
    projectTransform();
    viewTransform();

}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation (program->programId(), "proj"); //Enviem al shader el uniform de la matriu de projecció
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

MyGLWidget::~MyGLWidget() {
}



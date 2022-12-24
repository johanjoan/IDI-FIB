
//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
  glViewport (0, 0, ample/2, alt/2); //Canviem mida del viewport
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  modelTransform();

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 9); //Ara pintarem 9 vertexs (6 del quadrat i 3 del triangle)
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);

  // glBindVertexArray(0);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3); //Ara pintarem 3 vertexs
  
  // Desactivem el VAO
  glBindVertexArray(0);

  Rot = -Rot; //Per a que la següent TG faci que es roti en sentit contrari
  modelTransform();
  //Pintem en un nou viewport començant per ample/2, alt/2
  glViewport (ample/2, alt/2, ample/2, alt/2); //Canviem mida del viewport
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 9); //Ara pintarem 9 vertexs (6 del quadrat i 3 del triangle)
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3); //Ara pintarem 3 vertexs 

  // Desactivem el VAO
  glBindVertexArray(0);

  Rot = -Rot;  //Recuperem la rotació inicial

}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::creaBuffers ()    //Modificaem els vèrtexs 
{  
  glm::vec3 Vertices[9];
  //Quadrat de la casa
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(0.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 0.0, 0.0);

  Vertices[3] = glm::vec3(-1.0,-1.0,0.0);
  Vertices[4] = glm::vec3(-1.0,0.0,0.0);
  Vertices[5] = glm::vec3(0.0,0.0,0.0);

  //Triangle superior

  Vertices[6] = glm::vec3(-1.0,0.0,0.0);
  Vertices[7] = glm::vec3(-0.5,1.0,0.0);
  Vertices[8] = glm::vec3(0.0,0.0,0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  //Creem un nou VAO
  glm::vec3 triangle[3];

  triangle[0] = glm::vec3(0.0, 0.0, 0.0);
  triangle[1] = glm::vec3(0.5, 1.0, 0.0);
  triangle[2] = glm::vec3(1.0, 0.0, 0.0);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2); //Generem identificador
  glBindVertexArray(VAO2); //Activem el VAO

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO2;
  glGenBuffers(1, &VBO2);  //Generem identificadors per al VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO2); //Activem el VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW); //Enviem dades a la GPU
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  // Desactivem el VAO
  glBindVertexArray(0);


}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  transLoc = glGetUniformLocation (program->programId(), "TG");

}


void MyGLWidget::keyPressEvent (QKeyEvent *e) {
  makeCurrent (); //Necessari sempre que es vulgui detectar un events
  switch ( e->key() ) {  //key() ens indica quina tecla s'ha premut
    case Qt::Key_P:
      Rot += M_PI/6.;
      break;
    default: e->ignore (); // propagar al pare
  }
  update (); //Fen update per a que es torni a dibuixar (GlDraw). Ara ja tindrem el valor de scl serà diferent
}

void MyGLWidget::modelTransform () {  //Igual que exercici 2 pero ara canviant l'ordre de les transformacions
  glm::mat4 TG (1.0); // Matriu de transformació, inicialment identitat
  TG = glm::rotate(TG,Rot,glm::vec3(0.0,0.0,1.0)); //Rotació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
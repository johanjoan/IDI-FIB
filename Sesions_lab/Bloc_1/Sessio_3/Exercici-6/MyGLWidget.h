#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //Incloure funcions transformació geometrica
#include <QKeyEvent> //Carrega detecció events teclat

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    //Funció per a que passi algo quan apretem una tecla
    virtual void keyPressEvent ( QKeyEvent * e );

  private:
    void creaBuffers ();
    void carregaShaders ();
    void modelTransform();
    
    // attribute locations
    GLuint vertexLoc;
    GLuint transLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1; //VAO casa
    GLuint VAO2; // VAO Triangle extra
    GLint ample, alt;
    float Rot = 0.0;
};

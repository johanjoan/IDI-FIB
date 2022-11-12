// MyGLWidget.h
#include "BL2GLWidget.h"
#include "Model/model.h"
#include <QMouseEvent>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    void creaBuffers();
    void paintGL();
    void initializeGL();
    void viewTransform();
    void ini_camera();
    void projectTransform();
    void carregaShaders();
    void modelTransformPatricio();
    void modelTransformTerra();
    void keyPressEvent (QKeyEvent *event);
    void parametres_escena(glm::vec3 punt_max,glm::vec3 punt_min);
    void resizeGL(int w, int h);
    void calcula_escena();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

  private:
    int printOglError(const char file[], int line, const char func[]);
    Model m;
    GLuint VAO1,VAOTerra;
    GLuint viewLoc;
    GLuint projLoc;
    float FOV,FOVini, ra, znear, zfar; //Paràmetres PM
    glm::vec3 obs,vrp,up; //Paràmetres VM
    float rotation = 0.0f; //Rotació inicial del Patricio
    glm::vec3 centre_escena;
    float radi,radi_ample,radi_alt;
    glm::vec3 punt_max,punt_min;//Punts maxims i mínims de l'escena
    glm::vec3 base_patricio;
    float escala_patricio;
    int opt = 1; //1 == perspectiva; 0 == ortogonal
    float theta = 0; //Angle eix y
    float phi = 0; //Angle eix x
    float dist=0;  //Distancia desde la camera fins a la esfera contenedora
    float x_old,x_new,y_old,y_new; //Posicions del mouse
};


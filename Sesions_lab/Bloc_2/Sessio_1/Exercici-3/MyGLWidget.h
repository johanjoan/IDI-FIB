// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();
    void projectTransform(); //Implllementa crida a perspective
    void carregaShaders(); //Afegirem el enviament de la matriu de projecció al vertex shader
    void paintGL();
    void viewTransform(); //View Matrix
    void ini_camera();

  private:
    int printOglError(const char file[], int line, const char func[]);
    GLuint projLoc; //Uniform PM
    GLuint viewLoc; //Uniform VM
    float FOV, ra, znear, zfar; //Paràmetres PM
    glm::vec3 obs,vrp,up;

};

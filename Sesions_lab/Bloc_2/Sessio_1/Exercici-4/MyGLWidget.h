// MyGLWidget.h
#include "BL2GLWidget.h"
#include "Model/model.h"

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

  private:
    int printOglError(const char file[], int line, const char func[]);
    Model m;
    GLuint VAO1;
    GLuint viewLoc;
    GLuint projLoc;
    float FOV, ra, znear, zfar; //Paràmetres PM
    glm::vec3 obs,vrp,up; //Paràmetres VM
};


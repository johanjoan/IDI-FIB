// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void iniMaterialTerra ();
    virtual void initializeGL();
  private:
    int printOglError(const char file[], int line, const char func[]);
    GLuint pos_focus_Loc,col_focus_Loc,pos_cam_Loc;
    float x_focus = 1.0;
    glm::vec3 pos_focus_escena = glm::vec3(1.0,1.0,1.0);  //Focus Escena
    glm::vec3 pos_focus_camera = glm::vec3(0.0,0.0,0.0); //Focus Càmera
};

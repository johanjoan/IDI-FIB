#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void RickTransform();
    virtual void PortalTransform();
    virtual void iniCamera();
    virtual void projectTransform ();
    virtual void viewTransform();
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:

    int printOglError(const char file[], int line, const char func[]);
    float fovini,dist;
    glm::vec3 punt_max,punt_min;//Punts maxims i mínims de l'escena
    float theta = M_PI/4.0; //Valor inicial per veure l'escena inclinada 45º
    float psi = 0;
};

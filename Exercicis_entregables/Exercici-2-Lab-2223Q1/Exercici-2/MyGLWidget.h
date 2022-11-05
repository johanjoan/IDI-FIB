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
    virtual void MortyTransform();
    virtual void iniCamera();
    virtual void projectTransform ();
    virtual void viewTransform();
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void keyPressEvent (QKeyEvent *event);

  private:

    int printOglError(const char file[], int line, const char func[]);
    float fovini,dist;
    glm::vec3 punt_max,punt_min;//Punts maxims i mínims de l'escena
    float theta = M_PI/4.0; //Valor inicial per veure l'escena inclinada 45º
    float psi = 0;
    float angle_Rick = M_PI;
    float angle_Portal = M_PI;
    float angle_Morty = 0;
    glm::vec3 posicio_Rick = glm::vec3(-2.5,0,0);
    glm::vec3 Mirada_Rick[8] = {
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0,  -1.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0,  1.0),
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(-1.0, 0.0, 1.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0, -1.0)
    };
    glm::vec3 Mirada_Morty[8] = {
        glm::vec3(0.0, 0.0, 1.0),
        glm::vec3(-1.0, 0.0,  1.0),
        glm::vec3(-1.0, 0.0, 0.0),
        glm::vec3(-1.0, 0.0,  -1.0),
        glm::vec3(0.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, -1.0),
        glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(1.0, 0.0, 1.0)
    };
    int rick_mira = 0;
    int morty_mira = 0;
    glm::vec3 posicio_Portal = posicio_Rick + Mirada_Rick[rick_mira]*float(3.0); 
    glm::vec3 posicio_Portal_nova = posicio_Portal;
};

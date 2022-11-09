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
    virtual void creaBuffersModels();
    virtual void paintGL();
    virtual void carregaShaders();
    virtual bool MortyPortalTest();
    void MortyTransform2();
    void Portal_passat();

  private:

    int printOglError(const char file[], int line, const char func[]);
    float fovini,dist;
    glm::vec3 punt_max,punt_min;//Punts maxims i mínims de l'escena
    float theta = M_PI/4.0; //Valor inicial per veure l'escena inclinada 45º
    float psi = 0;
    float angle_Rick = M_PI;
    float angle_Portal = M_PI;
    glm::vec3 posicio_Rick = glm::vec3(-2.5,0,0);
    glm::vec3 posicio_Portal = glm::vec3(sin(angle_Rick),0.0,cos(angle_Rick))*glm::vec3(3) + posicio_Rick; 
    glm::vec3 posicio_Portal_nova = posicio_Portal;
    Model nou_Morty;
    GLuint VAO_nou_Morty;

    //Per coloreja el nou Morty
    GLuint alternatiuLoc;
    float alternatiu = 1;
    int actualitzar_Morty_nou = 0;
    int camera = 0; //Camera desde fora si 0, cap Morty si 1
};

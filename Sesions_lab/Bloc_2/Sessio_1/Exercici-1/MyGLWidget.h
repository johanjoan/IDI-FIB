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

  private:
    int printOglError(const char file[], int line, const char func[]);
    GLuint projLoc;
    
};

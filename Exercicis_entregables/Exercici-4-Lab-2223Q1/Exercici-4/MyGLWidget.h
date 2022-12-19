// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void modelTransformMorty();
    virtual void modelTransformFantasma();
    void restart();
    GLuint colorLoc,focusLoc,llumMortyLoc,colorllantLoc;
  private:
    int printOglError(const char file[], int line, const char func[]);
    glm::vec3 pos_llum_morty = glm::vec3(60,-90,-7); //Posició de la llum del focus del morty en SC Model
    glm::vec3 color_llanterna; //Color del llum de la llanterna
    glm::vec3 color_escena; //Color dels focus d'escena
    float angle_morty = 90;
    bool llanterna_encesa = true;
    bool llum_escena = true;
    bool morti_mira = false;
    float xFantasma = 0.5;
};


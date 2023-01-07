#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void set_camera(bool camera);
    void set_cub_patricio(int cub);

  signals:
    void canvi_camera1(bool camPlanta);
    void canvi_camera2(bool camPlanta);
    void num_cub(int cub);

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();

  private:
    int printOglError(const char file[], int line, const char func[]);
    float angle_pos_patricio = 0.0; //Inicialment en el cub-1
    float angle_cubs = 0.0;
    bool to_print = true; //true-> pinta els cubs. False-> pinta el patricio
    bool col_foc = true; //true -> focus blanc. False -> focus groc
};

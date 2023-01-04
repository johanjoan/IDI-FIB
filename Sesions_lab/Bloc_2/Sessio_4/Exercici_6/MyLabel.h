#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

    public:
        MyLabel(QWidget *parent);
    
    public slots:
        void set_r(int valor);
        void set_g(int valor);
        void set_b(int valor);
        void set_background();
    signals:
        void envia_background(int r, int g, int b);

    private:
        int r,g,b;
};
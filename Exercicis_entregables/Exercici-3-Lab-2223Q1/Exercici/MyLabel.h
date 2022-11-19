#include <QLabel>
#include <cmath>

class MyLabel: public QLabel {
    Q_OBJECT

    public:
    MyLabel(QWidget *parent);

    public slots:
        void set_nota_parcial(double nota);
        void set_nota_final(double nota);
        void set_nota_lab(double nota);
        void set_extra_lab(bool ticked);
        void reset();
        
    signals:
        void neteja(const double &);
        void nota_necessaria(const double &);

    private:
        void calcula_nota();
        double nota_parcial = 0;
        double nota_final = 0;
        double nota_lab = 0;
        double nota_total = 0;
        double extra_lab = 0;
        int entregues = 0;
};
#include "MyLabel.h"


MyLabel::MyLabel(QWidget *parent): QLabel(parent){
    this->installEventFilter(this);
}


void MyLabel::set_nota_parcial(double nota) {
    nota_parcial = nota;
    MyLabel::calcula_nota();
}

void MyLabel::set_nota_final(double nota) {
    nota_final = nota;
    MyLabel::calcula_nota();
}

void MyLabel::set_nota_lab(double nota) {
    nota_lab = nota;
    MyLabel::calcula_nota();
}

void MyLabel::calcula_nota() {
    if (nota_lab < 4.5) nota_total = 0.25*nota_parcial + 0.5*nota_final + 0.25*nota_lab;
    else nota_total = 0.25*nota_parcial + 0.5*nota_final + 0.25*std::min(10.0,nota_lab+extra_lab);
    setNum(nota_total);
    if (nota_total < 5) setStyleSheet("color: red;");
    else setStyleSheet("color: green;");

    if (nota_final == 0.0) emit nota_necessaria((5.0 - (0.25*nota_parcial + 0.25*nota_lab))/0.5);
    else emit nota_necessaria(0.0);
}

void MyLabel::reset() {
    emit neteja(0.0);
}

void MyLabel::set_extra_lab(bool ticked) {
    if (ticked) ++entregues;
    else --entregues;
    extra_lab = ceil(pow(entregues/4.0,2.2)*100.0)/100.0;
    calcula_nota();
}


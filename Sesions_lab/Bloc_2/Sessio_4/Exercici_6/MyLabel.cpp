#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent): QLabel(parent){
    r = 0;
    g = 0;
    b = 0;
}

void MyLabel::set_r(int valor) {
    r = valor;
    set_background();
}

void MyLabel::set_g(int valor) {
    g = valor;
    set_background();
}

void MyLabel::set_b(int valor) {
    b = valor;
    set_background();
}

void MyLabel::set_background() {
    setStyleSheet(QString("QLabel { background-color:rgb(%1,%2,%3)}").arg(r).arg(g).arg(b));
    emit envia_background(r, g, b);
}



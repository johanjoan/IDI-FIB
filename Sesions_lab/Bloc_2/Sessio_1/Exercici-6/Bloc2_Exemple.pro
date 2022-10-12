TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /usr/include/glm

FORMS += MyForm.ui

HEADERS += MyForm.h BL2GLWidget.h MyGLWidget.h

SOURCES += main.cpp MyForm.cpp \
        BL2GLWidget.cpp MyGLWidget.cpp

INCLUDEPATH += /home/joan/Escriptori/fib/IDI/Sesions_lab/Bloc_2/Sessio_1/Exercici-4/Model

SOURCES += /home/joan/Escriptori/fib/IDI/Sesions_lab/Bloc_2/Sessio_1/Exercici-4/Model/model.cpp
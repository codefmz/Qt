#ifndef CONTEND_H
#define CONTEND_H

#include <QDialog>
#include <QFrame>
#include <QPushButton>
#include <QStackedWidget>
class Contend : public QFrame
{
public:
    Contend(QWidget * parent = 0);
    QStackedWidget * stack;
    QPushButton * AmendBtn;
    QPushButton * CloseBtn;


};

#endif // CONTEND_H

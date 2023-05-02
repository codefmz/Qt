#ifndef TESTO_H
#define TESTO_H
#include <QObject>

class TestO : public QObject
{
    Q_OBJECT
public:
    TestO();
public slots:
    void test();
};

#endif // TESTO_H

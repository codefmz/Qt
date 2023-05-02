#include "TestO.h"
#include<QDebug>
TestO::TestO()
{

}

void TestO::test()
{
    qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" --  TestO without inhert QObject slots";
}

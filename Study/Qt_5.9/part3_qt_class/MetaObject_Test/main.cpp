#include "Widget.h"
#include <QApplication>
#include <QDebug>
#define QT_NO_DEBUG_OUTPUT

QStringList getStrings(QString value)
{
    QStringList res;
    int         blocks = 0;
    bool        escaped = false;
    char        str_char = 0;
    int         start = 0;
    int         n = 0;

    for (auto& c : value)
    {
        ++n;

        if (escaped)
        {
            escaped = false;
            continue;
        }
        else if (str_char)
        {
            if (c == str_char)
            {
                str_char = 0;
            }
            else if (c == '\\')
            {
                escaped = true;
            }
            continue;
        }

        switch (c.unicode())
        {
        case '{':
        case '(':
        case '[':
            ++blocks;
            break;

        case '}':
        case ')':
        case ']':
            --blocks;
            break;

        case '\\':
            escaped = true;
            break;

        case '"':
        case '\'':
            str_char = c.unicode();
            break;

        case ',':
            if (blocks == 0)
            {
                res.append(value.mid(start, n - 1).trimmed());
                start += n;
                n = 0;
            }
            break;
        }
    }

    if (n > 0)
    {
        // Append the rest of the string if there's anything left.
        res.append(value.mid(start).trimmed());
    }
    else if (!value.isEmpty() && value[value.size() - 1] == ',')
    {
        // Append an empty string if the string ends with ,
        res.append(QString());
    }

    return res;
}

QStringList getStrings2(QString string)
{
    return string.mid(1, string.length() - 2).split(',');
}
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
//    const QMetaObject * meta = w.metaObject();
    //测试元对象系统
//    Test::testMetaObject(w, meta);
//    //测试属性系统
//    Test::testProperty(w,meta);
//    //测试版本宏
//    Test::testMacro();

    //测试容器
//    Test::testContainer();

    QString str = "{31.3202,23.8446},{2,3}";
    QStringList s = getStrings(str);
    qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- s = " << s ;




    return a.exec();
}

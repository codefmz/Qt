#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString myname MEMBER myname)
    Q_CLASSINFO("author", "Feng");
    Q_CLASSINFO("company", "vf");
    Q_CLASSINFO("version", "3.0.1");

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

    const QString& name() const;
private slots:
    void testSlot();

private:
    Ui::Widget* ui;
    QString     myname;

};

namespace Test
{
    void testMetaObject(Widget &w, const QMetaObject * meta);
    void testProperty(Widget &w, const QMetaObject * meta);
    void testMacro();
    void testContainer();
}
#endif // WIDGET_H

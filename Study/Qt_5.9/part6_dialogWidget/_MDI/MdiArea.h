#ifndef MDIAREA_H
#define MDIAREA_H

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTabBar>
class MdiArea : public QMdiArea
{
    Q_OBJECT
public:
    MdiArea();

    bool eventFilter(QObject *watched, QEvent *event);
private:
    QTabBar * tabBar;
};

#endif // MDIAREA_H

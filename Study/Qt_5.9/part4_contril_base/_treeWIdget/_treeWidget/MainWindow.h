#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QLabel>
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QPixmapCache>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_actView_triggered(bool checked);

    void on_actFloat_triggered(bool checked);

    void on_actAddFile_triggered();

    void on_dockWidget_visibilityChanged(bool visible);

    void on_dockWidget_topLevelChanged(bool topLevel);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_actHeight_triggered();

    void on_actionWidth_triggered();

    void on_actSmall_triggered();

    void on_actBig_triggered();

    void on_actRealSize_triggered();

    void on_actDelItem_triggered();

    void on_actTraverse_triggered();

    void update();

    void on_actAddFold_triggered();

protected:
    void resizeEvent(QResizeEvent *ev) override;

private:
    Ui::MainWindow* ui;
    QLabel * fileNameLabel;
    QPixmap  curPixmap;
    qreal pixRatio;
    QPixmapCache pixmapCache;
    QTimer timer;
    int traverseIndex;
    QList<QTreeWidgetItem*> imageItemList;
    enum TreeColNum //表示列号
    {
        COLITEM = 0,
        COLITEMTYPE
    };
    enum TreeItemType
    {
        TOP = 1001,
        GROUP,
        IMAGE
    };

    void initTree();
    void addImageItem(QTreeWidgetItem * parItem, const QString & fileName);
    void displayImage(const QString & fileName);
    void changeActStatus(bool isEnable);
    void addFolderItem(QTreeWidgetItem * parItem, const QString & dir);
};
#endif // MAINWINDOW_H

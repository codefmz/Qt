#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    treeView = new QTreeView;

    tableView = new QTableView;

    listView = new QListView;

    QVBoxLayout * hBLayout = new QVBoxLayout;

    QGroupBox * groupBox1 = new QGroupBox;
    QVBoxLayout * vBL1 = new QVBoxLayout;
    vBL1->addWidget(treeView, -1);
    vBL1->setMargin(4);
    groupBox1->setLayout(vBL1);
    groupBox1->setTitle(tr("TreeView"));
    QGroupBox * groupBox2 = new QGroupBox;
    QVBoxLayout * vBL2 = new QVBoxLayout;
    vBL2->addWidget(tableView, -1);
    vBL2->setMargin(4);
    groupBox2->setLayout(vBL2);
    groupBox2->setTitle(tr("TableView"));
    QGroupBox * groupBox3 = new QGroupBox;
    groupBox3->setTitle(tr("ListView"));
    QVBoxLayout * vBL3 = new QVBoxLayout;
    vBL3->addWidget(listView, -1);
    vBL3->setMargin(4);
    groupBox3->setLayout(vBL3);
    nameLabel = new QLabel(tr("名字: "));
    sizeLabel = new QLabel(tr("大小: "));
    fileFolderLabel = new QLabel(tr("File Folder: "));
    pathLabel = new QLabel(tr("路径: "));
    dirCkBox = new QCheckBox(tr("节点是目录"));


//    QGridLayout * gridLayout = new QGridLayout;

//    gridLayout->addWidget(groupBox1, 0, 0,2,1);
//    gridLayout->addWidget(groupBox3, 0, 1,1,1);
//    gridLayout->addWidget(groupBox2, 1, 1,1,1);

    QSplitter * mainSplitter = new QSplitter(this);
    mainSplitter->setOrientation(Qt::Horizontal);
    QSplitter * splitter = new QSplitter(mainSplitter);
    splitter->setOrientation(Qt::Vertical);

    mainSplitter->addWidget(groupBox1);
    mainSplitter->addWidget(splitter);
    splitter->addWidget(groupBox3);
    splitter->addWidget(groupBox2);

    QHBoxLayout * hBoxLayout = new QHBoxLayout;
    hBoxLayout->addWidget(nameLabel);
    hBoxLayout->addSpacing(2);
    hBoxLayout->addWidget(sizeLabel);
    hBoxLayout->addWidget(fileFolderLabel);
    hBoxLayout->addWidget(dirCkBox);

    QGroupBox * groupBox4 = new QGroupBox;
    QVBoxLayout * vBL4 = new QVBoxLayout;
    vBL4->addLayout(hBoxLayout);
    vBL4->addWidget(pathLabel);
    vBL4->setMargin(4);
    groupBox4->setLayout(vBL4);

//    gridLayout->addLayout(hBoxLayout, 2, 0, 1, -1);
//    gridLayout->addWidget(pathLabel, 3, 0, 1, -1);
//    gridLayout->setRowStretch(0,4);
//    gridLayout->setRowStretch(1,4);
//    gridLayout->setRowStretch(2,1);
//    gridLayout->setRowStretch(3,1);
//    gridLayout->setColumnStretch(0,1);
//    gridLayout->setColumnStretch(1,1);
//    setLayout(gridLayout);

    QSplitter * splitter2 = new QSplitter(Qt::Vertical);
    splitter2->addWidget(mainSplitter);
    splitter2->addWidget(groupBox4);

//    splitter2->setSizes(QList<int>()<<1/3*width()<<2/3*width());

    hBLayout->addWidget(splitter2);

    groupBox1->setMinimumHeight(200);
    groupBox1->setMinimumWidth(200);
    setLayout(hBLayout);
    systemModel = new QFileSystemModel(this);

    systemModel->setRootPath(QDir::currentPath());
    treeView->setModel(systemModel);
    listView->setModel(systemModel);
    tableView->setModel(systemModel);
    tableView->verticalHeader()->setVisible(false);

    connect(treeView, SIGNAL(clicked(QModelIndex)), listView, SLOT(setRootIndex(QModelIndex)));
    connect(treeView, SIGNAL(clicked(QModelIndex)), tableView, SLOT(setRootIndex(QModelIndex)));
}

Widget::~Widget()
{
    delete ui;
}


#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "QDebug"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("UserInfo"));

    /**************左侧*************/
    UserNameLabel = new QLabel(tr("用户名："));
    UserNameLineEdit = new QLineEdit;
    NameLabel = new QLabel(tr("姓名："));
    NameLineEdit = new QLineEdit;
    SexLabel = new QLabel(tr("性别："));
    SexComboBox = new QComboBox;
    SexComboBox->addItem(tr("女"));
    SexComboBox->addItem(tr("男"));
    DepartmentLabel = new QLabel(tr("部门："));
    DepartmentTextEdit = new QTextEdit;
    AgeLabel = new QLabel(tr("年龄："));
    AgeLineEdit = new QLineEdit;
    OtherLabel = new QLabel(tr("其他："));
    OtherLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); //设置控件的风格。 包括两种，阴影和形状 setFrameStyle

    LeftLayout = new QGridLayout;
    //用户名
    LeftLayout->addWidget(UserNameLabel,0,0);
    LeftLayout->addWidget(UserNameLineEdit,0,1);
    //姓名
    LeftLayout->addWidget(NameLabel,1,0);
    LeftLayout->addWidget(NameLineEdit,1,1);
    //性别
    LeftLayout->addWidget(SexLabel,2,0);
    LeftLayout->addWidget(SexComboBox,2,1);
    //部门
    LeftLayout->addWidget(DepartmentLabel,3,0);
    LeftLayout->addWidget(DepartmentTextEdit,3,1);
    //年龄
    LeftLayout->addWidget(AgeLabel, 4,0);
    LeftLayout->addWidget(AgeLineEdit, 4,1);
    //其他
    LeftLayout->addWidget(OtherLabel,5,0, 1,2);

    LeftLayout->setColumnStretch(0,1);
    LeftLayout->setColumnStretch(1,3);

    //右侧
    HeadLabel = new QLabel(tr("头像："));
    HeadIconLabel = new QLabel;
    HeadIconLabel->resize(108,92);
    QPixmap icon(":/view.jpg");


//    HeadIconLabel->resize(20,10);

    icon = icon.scaled(HeadIconLabel->size());

    HeadIconLabel->setPixmap(icon);
    qDebug() << "HeadIconLabel->size() = " << HeadIconLabel->size();
    UpdateHeadBtn = new QPushButton(tr("更新"));

    TopRightLayout = new QHBoxLayout;
    TopRightLayout->setSpacing(20);
    TopRightLayout->addWidget(HeadLabel);
    TopRightLayout->addWidget(HeadIconLabel);
    TopRightLayout->addWidget(UpdateHeadBtn);
    IntroductionLabel = new QLabel(tr("个人说明："));
    IntroductionTextEdit = new QTextEdit;

    RightLayout = new QVBoxLayout;
    RightLayout->setMargin(10);
    RightLayout->addLayout(TopRightLayout);
    RightLayout->addWidget(IntroductionLabel);
    RightLayout->addWidget(IntroductionTextEdit);

    /**************底部*************/
    OkBtn = new QPushButton(tr("确定"));
    CancelBtn = new QPushButton(tr("取消"));
    ButtomLayout = new QHBoxLayout;
    ButtomLayout->addStretch(); //在按钮之前添加一个占位符
    ButtomLayout->addWidget(OkBtn);
    ButtomLayout->addWidget(CancelBtn);


    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setMargin(15); //设置对话框的边距为 15
    mainLayout->setSpacing(10); //设置控件之前的间距
    mainLayout->addLayout(LeftLayout,0,0);
    mainLayout->addLayout(RightLayout, 0,1);
    mainLayout->addLayout(ButtomLayout,1,0,1,2);
//    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    QWidget * widget = new QWidget;
    widget->setLayout(mainLayout);
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


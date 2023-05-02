#include "Dialog.h"

#include "QButtonGroup"
#include "ui_Dialog.h"

#include <QDebug>
Dialog::Dialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(oncolorRadioButtonClicked()));
    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(oncolorRadioButtonClicked()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(oncolorRadioButtonClicked()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_checkBox_clicked(bool checked)

{
    QFont font = ui->plainTextEdit->font();
    font.setUnderline(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::on_checkBox_2_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setItalic(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::on_checkBox_3_clicked(bool checked)
{
    QFont font = ui->plainTextEdit->font();
    font.setBold(checked);
    ui->plainTextEdit->setFont(font);
}

void Dialog::oncolorRadioButtonClicked()
{
    QPalette palette = ui->plainTextEdit->palette();
    qDebug().nospace() << __FILE__ << "(" << __LINE__ << ")" << __FUNCTION__ << " -- " << sizeof(palette);
    if (ui->radioButton->isChecked())
    {
        palette.setColor(QPalette::Text, Qt::black);
    }
    else if (ui->radioButton_2->isChecked())
    {
        palette.setColor(QPalette::Text, Qt::red);
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::blue);
    }
    ui->plainTextEdit->setPalette(palette);
}

#include "add.h"
#include "ui_add.h"
#include <QRegExp>
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QMessageBox>
#include <QDoubleValidator>

add::add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 500, 500));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();
    ui->okButton->setEnabled(false);

    QRegExp regExp1("[0-9]{6}");
    ui->onumEdit->setValidator(new QRegExpValidator(regExp1, this));
    ui->ocreditEdit->setValidator(new QDoubleValidator());

    connect(ui->onumEdit, SIGNAL(textChanged(QString)), this, SLOT(setOkButton()));
    connect(ui->onameEdit, SIGNAL(textChanged(QString)), this, SLOT(setOkButton()));
    connect(ui->ocreditEdit, SIGNAL(textChanged(QString)), this, SLOT(setOkButton()));
}

add::~add()
{
    delete ui;
}

void add::setOkButton()
{
    ui->okButton->setEnabled(true);
}

void add::on_okButton_clicked()
{
    QString g2g4No = ui->onumEdit->text();
    QString g2g4Name = ui->onameEdit->text();
    float g2g4Credit = ui->ocreditEdit->text().toFloat();
    QString g2g4Attr = "g3+";
    if (ui->radiog3->isChecked())
        g2g4Attr = "g3+";
    if (ui->radiog4->isChecked())
        g2g4Attr = "g4+";
    if (ui->radiog5->isChecked())
        g2g4Attr = "g5+";
    if (ui->radiog6->isChecked())
        g2g4Attr = "g6-";

    QSqlQuery queryaddoItem(tr("SELECT * FROM gOperator WHERE "
                            "g2g4No = '%1'").arg(g2g4No));
    int numquery = 0;

    while (queryaddoItem.next())
    {
        numquery++;
    }
    //ui->Edit->setText(QString::number(numquery));

    if (numquery == 0){
        queryaddoItem.exec(QString("INSERT INTO gOperator "
              "VALUES('%1', '%2', %3, '%4')").arg(g2g4No).arg(g2g4Name).arg(g2g4Credit).arg(g2g4Attr));
    }
    else if (numquery > 0)
    {
        QMessageBox::warning(this, tr("添加加减分项"),
                            tr("该项(编号%1), %2加减分项已存在")
                            .arg(g2g4No).arg(g2g4Name));
    }
    this->close();
}

void add::on_onumEdit_returnPressed()
{
    ui->okButton->click();
}

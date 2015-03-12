#include <QtGui>
#include "setting.h"
#include "ui_setting.h"
#include "QtSql"
#include <QDataWidgetMapper>
#include <QSqlQuery>

Setting::Setting(QString id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 2000, 1414));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();

    ui->gBox->hide();
    QRegExp regExp1("[0-9]{0,11}");
    ui->phonenumEdit->setValidator(new QRegExpValidator(regExp1, this));
    QRegExp regExp2("[0-9]{0,11}");
    ui->qqEdit->setValidator(new QRegExpValidator(regExp2, this));
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    QSqlQuery findClassNo;
    findClassNo.exec(QString("Select * from student where studentNo = '%1'").arg(id));
    QString classNo = "";
    while (findClassNo.next())
    {
        classNo = findClassNo.value(Student_classNo).toString();
    }

    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("student");
    tableModel->setFilter(QString("class='%1'").arg(classNo));
    tableModel->setSort(Student_classNo, Qt::AscendingOrder);
    tableModel->select();
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->nameEdit, Student_name);
    mapper->addMapping(ui->sexEdit, Student_sex);
    mapper->addMapping(ui->classEdit, Student_classNo);
    mapper->addMapping(ui->stunumEdit, Student_No);
    mapper->addMapping(ui->phonenumEdit, Student_phone);
    mapper->addMapping(ui->emailEdit, Student_email);
    mapper->addMapping(ui->qqEdit, Student_QQ);
    mapper->addMapping(ui->dormitoryEdit, Student_domitory);
    mapper->addMapping(ui->g1Edit,Student_G1);
    mapper->addMapping(ui->g2Edit,Student_G2);
    mapper->addMapping(ui->g3Edit,Student_G3);
    mapper->addMapping(ui->g4Edit,Student_G4);
    mapper->addMapping(ui->g5Edit_2,Student_G5);
    mapper->addMapping(ui->g6Edit_2,Student_G6);
    mapper->addMapping(ui->gEdit,Student_G);
    if (id != "0000000000") {
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(Student_No).toString() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }
    connect(ui->firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(ui->previousButton, SIGNAL(clicked()),
            mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(ui->lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
}

void Setting::done(int result)
{

    mapper->submit();
    QDialog::done(result);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_saveButton_2_toggled(bool checked)
{
    if(checked==true)
    {
        ui->gBox->setVisible(true);
        ui->saveButton_2->setText(tr("收起"));
    }
    else
    {
        ui->gBox->setVisible(false);
        ui->saveButton_2->setText(tr("展开"));
    }
}

void Setting::on_phonenumEdit_textEdited(QString )
{
    ui->saveButton->setEnabled(true);
    ui->okButton->setEnabled(true);
}

void Setting::on_emailEdit_textEdited(QString )
{
    ui->saveButton->setEnabled(true);
    ui->okButton->setEnabled(true);
}

void Setting::on_qqEdit_textEdited(QString )
{
    ui->saveButton->setEnabled(true);
    ui->okButton->setEnabled(true);
}

void Setting::on_dormitoryEdit_textEdited(QString )
{
    ui->saveButton->setEnabled(true);
    ui->okButton->setEnabled(true);
}

void Setting::on_saveButton_clicked()
{
    ui->okButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
}

void Setting::on_okButton_clicked()
{
    close();
}

void Setting::on_emailEdit_returnPressed()
{
    ui->okButton->click();
}

void Setting::on_phonenumEdit_returnPressed()
{
    ui->okButton->click();
}

void Setting::on_qqEdit_returnPressed()
{
    ui->okButton->click();
}

void Setting::on_dormitoryEdit_returnPressed()
{
    ui->okButton->click();
}

#include "window.h"
#include "ui_window.h"
#include "setting.h"
#include <QVariant>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>
#include <QTableView>
#include <QMessageBox>
#include "statistics.h"
#include "showscore.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 2000, 1414));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();

    gradeModel = new QSqlRelationalTableModel(this);
    gradeModel->setTable("grade");
    gradeModel->setSort(0, Qt::AscendingOrder);
    gradeModel->select();
    ui->gradeBox->setModel(gradeModel);
    ui->gradeBox->setModelColumn(0);

    createClassView();
    createStudentView();

    connect(ui->classView->selectionModel(),
        SIGNAL(currentRowChanged(const QModelIndex &,
                                         const QModelIndex &)),
        this, SLOT(updateStudentView()));

    connect(ui->findcomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkFindEdit()));

    connect(ui->gradeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassView()));
    //connect(ui->findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
    connect(ui->editButton, SIGNAL(clicked()), this, SLOT(editStudent()));
    connect(ui->logoutButton,SIGNAL(clicked()),this,SIGNAL(logout()));
    connect(ui->rangeButton,SIGNAL(clicked()),this,SLOT(clickstatistic()));
    connect(ui->showScoreButton,SIGNAL(clicked()),this,SLOT(showScore()));

    connect(ui->studentView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showScore()));

}

void Window::createClassView()
{
    QSqlQuery query;
    classModel = new QSqlRelationalTableModel(this);
    classModel->setTable("class");
    classModel->setFilter(QString("grade = '%1'").arg(ui->gradeBox->currentText()));
    classModel->setSort(Class_No, Qt::AscendingOrder);
    classModel->setHeaderData(Class_No, Qt::Horizontal, tr("班级"));
    classModel->select();
    ui->classView->setModel(classModel);
    //ui->classView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->classView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->classView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Window::createStudentView()
{
    studentModel = new QSqlRelationalTableModel(this);
    studentModel->setTable("student");
    //studentModel->setRelation(Student_classNo,
    //        QSqlRelation("class", "classNo", "classNo"));

    studentModel->setSort(Student_No, Qt::AscendingOrder);
    studentModel->setHeaderData(Student_No, Qt::Horizontal, tr("学号"));
    studentModel->setHeaderData(Student_name, Qt::Horizontal, tr("姓名"));
    studentModel->setHeaderData(Student_majLanguage, Qt::Horizontal, tr("主修语言"));
    studentModel->setHeaderData(Student_major, Qt::Horizontal, tr("专业"));
    studentModel->setHeaderData(Student_grade, Qt::Horizontal, tr("年级"));
    studentModel->setHeaderData(Student_phone, Qt::Horizontal, tr("电话"));
    studentModel->setHeaderData(Student_domitory, Qt::Horizontal, tr("宿舍"));
    studentModel->setHeaderData(Student_email, Qt::Horizontal, tr("邮箱"));
    studentModel->setHeaderData(Student_QQ, Qt::Horizontal, tr("QQ号"));
    studentModel->setHeaderData(Student_sex, Qt::Horizontal, tr("性别"));
    studentModel->select();

    ui->studentView->setModel(studentModel);
    ui->studentView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->studentView->setSelectionMode(
             QAbstractItemView::SingleSelection);
    ui->studentView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->studentView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->studentView->resizeColumnsToContents();
    ui->studentView->setColumnHidden(Student_classNo, true);
    ui->studentView->setColumnHidden(Student_grade, true);
    ui->studentView->setColumnHidden(Student_sex, true);
    ui->studentView->horizontalHeader()->setStretchLastSection(true);
}

void Window::updateClassView()
{
    classModel->setFilter(QString("grade = '%1'").arg(ui->gradeBox->currentText()));
    classModel->select();
    ui->classView->setModel(classModel);
    //ui->classView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->classView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->classView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateStudentView();
}

void Window::updateStudentView()
{
    QModelIndex index = ui->classView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = classModel->record(index.row());
        QString classNo = record.value(Class_No).toString();
        studentModel->setFilter(QString("class = '%1'").arg(classNo));
    } else {
        QSqlRecord record = classModel->record(0);
        QString classNo = record.value(Class_No).toString();
        studentModel->setFilter(QString("class = '%1'").arg(classNo));
    }
    studentModel->select();
    ui->studentView->horizontalHeader()->setVisible(
            studentModel->rowCount() > 0);

}

void Window::editStudent()
{
    QSqlRecord record = studentModel->record(0);
    QString studentNo = record.value(0).toString();
    QModelIndex index = ui->studentView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = studentModel->record(index.row());
        studentNo = record.value(Student_No).toString();
    }

    Setting form(studentNo,this);
    form.exec();
    updateStudentView();
}


Window::~Window()
{
    delete ui;
}

void Window::checkFindEdit()
{
    int findindex = ui->findcomboBox->currentIndex();
    if (findindex == find_studentno) {
        QRegExp regExp1("[0-9]{0,10}");
        ui->findEdit->setValidator(new QRegExpValidator(regExp1, this));
    } else if (findindex == find_studentname) {
        ui->findEdit->setValidator(0);
    }
}

void Window::on_findButton_clicked()
{
    QString finddes = ui->findEdit->text();
    int findindex = ui->findcomboBox->currentIndex();
    int iffind = 0;
    if (findindex == 1) {
        QString studentNo = finddes;
        for (int row = 0; row < studentModel->rowCount(); ++row) {
            QSqlRecord record = studentModel->record(row);
            if (record.value(0).toString() == studentNo) {
                iffind = 1;

                QModelIndex index = ui->studentView->model()->index(row, 0);
                ui->studentView->setCurrentIndex(index);
                ui->studentView->selectRow(index.row());

                //ui->studentView->clicked(index);
                ui->studentView->scrollTo(index);
                break;
            }
        }
        if (iffind == 0)
        {
            QMessageBox::warning(this,tr("显示学生成绩"),
                           tr("找不到此学生: 学号") + studentNo, tr("确定"));
            ui->findEdit->clear();
        }
    } else if (findindex == 0){
        QString studentname = finddes;
        for (int row = 0; row < studentModel->rowCount(); ++row) {
            QSqlRecord record = studentModel->record(row);
            if (record.value(1).toString() == studentname) {
                iffind = 1;
                QModelIndex index = ui->studentView->model()->index(row, 0);
                ui->studentView->setCurrentIndex(index);
                ui->studentView->selectRow(index.row());

                //ui->studentView->clicked(index);
                ui->studentView->scrollTo(index);
                break;
            }
        }
        if (iffind == 0)
        {
            QMessageBox::warning(this,tr("显示学生成绩"),
                           tr("找不到此学生: 姓名") + studentname, tr("确定"));
            ui->findEdit->clear();
        }
    }
    studentModel->select();

    ui->studentView->horizontalHeader()->setVisible(
            studentModel->rowCount() > 0);

}

void Window::on_logoutButton_clicked()
{
   emit logout();
}

void Window::clickstatistic()
{
    emit statistic();
    /* statDialog;
    statDialog.raise();
    statDialog.activateWindow();
    statDialog.show();*/
}

void Window::showScore()
{
    QSqlRecord record = studentModel->record(0);
    QString studentNo = record.value(0).toString();
    QModelIndex index = ui->studentView->currentIndex();
    if (index.isValid())
    {
        QSqlRecord record = studentModel->record(index.row());
        studentNo = record.value(Student_No).toString();
    }

    showscore scoreDialog(studentNo);
    scoreDialog.raise();
    scoreDialog.activateWindow();
    scoreDialog.exec();
}

void Window::on_findEdit_returnPressed()
{
    ui->findButton->click();
}

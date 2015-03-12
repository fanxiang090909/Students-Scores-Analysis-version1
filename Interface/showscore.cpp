#include "showscore.h"
#include "ui_showscore.h"
#include <QSqlRelationalDelegate>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QMessageBox>
#include "change.h"

showscore::showscore(QString studNo, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::showscore)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 2000, 1414));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();

    QSqlQuery findClassNo;
    findClassNo.exec(QString("Select * from student where studentNo = '%1'").arg(studNo));
    QString classNo = "";
    while (findClassNo.next())
    {
        classNo = findClassNo.value(10).toString();
    }

    tableModel = new QSqlRelationalTableModel(this);
    tableModel->setTable("student");
    tableModel->setFilter(QString("class='%1'").arg(classNo));
    tableModel->setSort(0, Qt::AscendingOrder);
    tableModel->select();

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tableModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->stunumEdit, 0);
    mapper->addMapping(ui->nameEdit, 1);
    mapper->addMapping(ui->classEdit, 10);
    mapper->addMapping(ui->majorEdit, 11);
    mapper->addMapping(ui->g1Edit,2);
    mapper->addMapping(ui->g2Edit,3);
    mapper->addMapping(ui->g3Edit,4);
    mapper->addMapping(ui->g4Edit,5);
    mapper->addMapping(ui->g5Edit_2,6);
    mapper->addMapping(ui->g6Edit_2,7);
    mapper->addMapping(ui->gEdit,8);

    if (studNo != "0000000000") {
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(0).toString() == studNo) {
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

    connect(ui->findcomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkFindEdit()));

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addg()));

    createScoreShow(studNo);
}

void showscore::createScoreShow(QString studNo)
{
    scoreModel = new QSqlRelationalTableModel(this);
    scoreModel->setTable("coursescore");
    scoreModel->setFilter(QString("studentNo = %1").arg(studNo));
    scoreModel->setRelation(Score_courseNo,
            QSqlRelation("course", "courseNo", "courseName"));
    scoreModel->setSort(Score_courseNo, Qt::AscendingOrder);
    scoreModel->setHeaderData(Score_studNo, Qt::Horizontal, tr("学生"));
    scoreModel->setHeaderData(Score_courseNo, Qt::Horizontal, tr("课程名"));
    scoreModel->setHeaderData(Score_score, Qt::Horizontal, tr("成绩"));
    scoreModel->setHeaderData(Score_examDate, Qt::Horizontal, tr("考试日期"));
    scoreModel->setHeaderData(Score_time, Qt::Horizontal, tr("考试次数"));

    scoreModel->select();

    ui->scoreView->setModel(scoreModel);
    ui->scoreView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->scoreView->setSelectionMode(
             QAbstractItemView::NoSelection);
    ui->scoreView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->scoreView->resizeColumnsToContents();
    ui->scoreView->setColumnHidden(Score_studNo, true);
    ui->scoreView->horizontalHeader()->setStretchLastSection(true);

    gModel = new QSqlRelationalTableModel(this);
    gModel->setTable("gItem");

    updategmodel(studNo);

}

showscore::~showscore()
{
    delete ui;
}

void showscore::on_stunumEdit_textChanged(QString )
{
    createScoreShow(ui->stunumEdit->text());
    ui->findEdit->clear();

}

void showscore::checkFindEdit()
{
    int findindex = ui->findcomboBox->currentIndex();
    if (findindex == 1) {
        QRegExp regExp1("[0-9]{0,10}");
        ui->findEdit->setValidator(new QRegExpValidator(regExp1, this));
    } else if (findindex == 0)
        ui->findEdit->setValidator(0);

}

void showscore::on_findButton_clicked()
{
    QString finddes = ui->findEdit->text();
    int findindex = ui->findcomboBox->currentIndex();
    int iffind = 0;
    if (findindex == 1) {
        QString studentNo = finddes;
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(0).toString() == studentNo) {
                mapper->setCurrentIndex(row);
                iffind = 1;
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
        for (int row = 0; row < tableModel->rowCount(); ++row) {
            QSqlRecord record = tableModel->record(row);
            if (record.value(1).toString() == studentname) {
                mapper->setCurrentIndex(row);
                iffind = 1;
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
    tableModel->select();
}

void showscore::on_findEdit_textChanged(QString )
{
    ui->findButton->setEnabled(true);
}

void showscore::addg()
{
    QString studNo = ui->stunumEdit->text();
    change changeDialog(studNo);
    changeDialog.raise();
    changeDialog.activateWindow();
    changeDialog.exec();
    updategmodel(studNo);
}

void showscore::updategmodel(QString studNo)
{
    gModel->setFilter(QString("studentNo = %1").arg(studNo));
    gModel->setRelation(1,
            QSqlRelation("gOperator", "g2g4No", "g2g4Name"));
    gModel->setSort(1, Qt::AscendingOrder);
    gModel->setHeaderData(0, Qt::Horizontal, tr("学生"));
    gModel->setHeaderData(1, Qt::Horizontal, tr("加分减分记录"));
    gModel->setHeaderData(2, Qt::Horizontal, tr("参考学分"));
    gModel->select();

    ui->gView->setModel(gModel);
    ui->gView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->gView->setSelectionMode(
             QAbstractItemView::NoSelection);
    ui->gView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gView->resizeColumnsToContents();
    ui->gView->setColumnHidden(Score_studNo, true);
    ui->gView->horizontalHeader()->setStretchLastSection(true);
}

void showscore::on_findEdit_returnPressed()
{
    ui->findButton->click();
}

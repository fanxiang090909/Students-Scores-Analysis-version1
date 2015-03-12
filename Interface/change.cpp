#include <QDebug>
#include <QSqlRelationalDelegate>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QScrollArea>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlQuery>
#include "change.h"
#include "showscore.h"
#include "add.h"
#include "ui_change.h"

change::change(QString studNo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::change)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 2000, 1414));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();

    studentModel = new QSqlRelationalTableModel(this);
    studentModel->setTable("student");
    studentModel->select();
    studentModel->setFilter(QString("studentNo = '%1'").arg(studNo));

    gModel = new QSqlRelationalTableModel(this);
    gModel->setTable("gItem");

    creategmodel(studNo);
    createoperatorlist();

    studentModel = new QSqlRelationalTableModel(this);
    mapper = new QDataWidgetMapper(this);
    updatemapper(studNo);

    ui->saveg2Button->setEnabled(true);

    connect(ui->firstButton_2, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(ui->previousButton_2, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton_2, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(ui->lastButton_2, SIGNAL(clicked()), mapper, SLOT(toLast()));

    connect(ui->firstButton_2, SIGNAL(clicked()), this, SLOT(updategview()));
    connect(ui->previousButton_2, SIGNAL(clicked()), this, SLOT(updategview()));
    connect(ui->nextButton_2, SIGNAL(clicked()), this, SLOT(updategview()));
    connect(ui->lastButton_2, SIGNAL(clicked()), this, SLOT(updategview()));

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addgOperator()));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteg()));
    connect(ui->operatorView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addgItem(const QModelIndex &)));
    connect(ui->additemButton, SIGNAL(clicked()), this, SLOT(addgItem2()));
    connect(ui->gView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(deletegItem(const QModelIndex &)));
}

change::~change()
{
    delete ui;
}

void change::createoperatorlist()
{
    operatorModel = new QSqlRelationalTableModel(this);
    operatorModel->setTable("gOperator");
    //operatorModel->setFilter(QString("g2g4attr = '%1'").arg("+"));
    operatorModel->setSort(3, Qt::AscendingOrder);
    operatorModel->setHeaderData(0, Qt::Horizontal, tr("加减分项编号"));
    operatorModel->setHeaderData(1, Qt::Horizontal, tr("加减分项名称"));
    operatorModel->setHeaderData(2, Qt::Horizontal, tr("学分"));
    operatorModel->setHeaderData(3, Qt::Horizontal, tr("加减分属性"));
    operatorModel->select();

    ui->operatorView->setModel(operatorModel);
    ui->operatorView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->operatorView->setSelectionMode(
             QAbstractItemView::SingleSelection);
    ui->operatorView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->operatorView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->operatorView->resizeColumnsToContents();
    ui->operatorView->horizontalHeader()->setStretchLastSection(true);

}

void change::addgOperator()
{
    add addDialog(this);
    addDialog.raise();
    addDialog.activateWindow();
    addDialog.exec();
    createoperatorlist();
}

void change::addgItem(const QModelIndex & index)
{
    QString g2g4No = "0000000";
    QString studNo = ui->stunumEdit->text();
    if (index.isValid()) {
        QSqlRecord record = operatorModel->record(index.row());
        g2g4No = record.value(0).toString();
        QString g2g4Name = record.value(1).toString();
        QSqlQuery queryaddgItem(tr("SELECT * FROM gItem WHERE "
                                "studentNo = '%1' and gItem = '%2'").arg(studNo).arg(g2g4No));
        int numquery = 0;

        while (queryaddgItem.next())
        {
            numquery++;
        }
        if (numquery == 0){
            queryaddgItem.exec(QString("INSERT INTO gItem "
                  "VALUES('%1', '%2')").arg(studNo).arg(g2g4No));
        }
        else if (numquery > 0)
        {
            QMessageBox::warning(this, tr("添加学生加减分记录"),
                                tr("该学生(学号%1), %2加减分记录已存在")
                                .arg(studNo).arg(g2g4Name));
        }
    }
    updategview();
}

void change::addgItem2()
{
    QString g2g4No = "0000000";
    QString studNo = ui->stunumEdit->text();
    QModelIndex index = ui->operatorView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = operatorModel->record(index.row());
        g2g4No = record.value(0).toString();
        QString g2g4Name = record.value(1).toString();
        QSqlQuery queryaddgItem(tr("SELECT * FROM gItem WHERE "
                                "studentNo = '%1' and gItem = '%2'").arg(studNo).arg(g2g4No));
        int numquery = 0;

        while (queryaddgItem.next())
        {
            numquery++;
        }

        if (numquery == 0){
            queryaddgItem.exec(QString("INSERT INTO gItem "
                  "VALUES('%1', '%2')").arg(studNo).arg(g2g4No));
        }
        else if (numquery > 0)
        {
            QMessageBox::warning(this, tr("添加学生加减分记录"),
                                tr("该学生(学号%1), %2加减分记录已存在")
                                .arg(studNo).arg(g2g4Name));
        }
    }
    updategview();
}

void change::creategmodel(QString studNo)
{
    studentModel->setFilter(QString("studentNo = %1").arg(studNo));
    gModel->setFilter(QString("studentNo = %1").arg(studNo));
    gModel->setRelation(1,
            QSqlRelation("gOperator", "g2g4No", "g2g4Name"));
    gModel->setSort(1, Qt::AscendingOrder);
    gModel->setHeaderData(0, Qt::Horizontal, tr("学生"));
    gModel->setHeaderData(1, Qt::Horizontal, tr("加减分记录"));
    gModel->setHeaderData(2, Qt::Horizontal, tr("加减分项目学分"));
    gModel->setHeaderData(3, Qt::Horizontal, tr("加减分属性"));

    gModel->select();

    ui->gView->setModel(gModel);
    ui->gView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->gView->setSelectionMode(
             QAbstractItemView::SingleSelection);
    ui->gView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gView->resizeColumnsToContents();
    ui->gView->setColumnHidden(0, true);
    ui->gView->horizontalHeader()->setStretchLastSection(true);
}

void change::deleteg()
{
    QModelIndex index = ui->gView->currentIndex();
    if (!index.isValid())
        return;

    QSqlRecord record = gModel->record(index.row());
    QString g2g4Name = record.value(1).toString();
    QString studentNo = record.value(0).toString();
    QString g2g4No = "";

    QSqlQuery query1(QString("SELECT * FROM gItem "
                          "WHERE studentNo = '%1'").arg(studentNo));

    while (query1.next()) {
        g2g4No = query1.value(1).toString();
    }
    //qDebug() << studentNo << g2g4No << g2g4Name;
    int r = QMessageBox::warning(this, tr("删除加减分记录"),
                    tr("删除学生 %1, %2加减分记录?")
                    .arg(studentNo).arg(g2g4Name),
                     QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
        return;
    }

    query1.exec(QString("DELETE FROM gItem "
                           "WHERE studentNo = '%1' and gItem = '%2'").arg(studentNo).arg(g2g4No));

    updategview();
}

void change::deletegItem(const QModelIndex & index)
{
    if (!index.isValid())
        return;

    QSqlRecord record = gModel->record(index.row());
    QString g2g4Name = record.value(1).toString();
    QString studentNo = record.value(0).toString();
    QString g2g4No = "";

    QSqlQuery query1(QString("SELECT * FROM gItem "
                          "WHERE studentNo = '%1'").arg(studentNo));

    while (query1.next()) {
        g2g4No = query1.value(1).toString();
    }
    int r = QMessageBox::warning(this, tr("删除加减分记录"),
                    tr("删除学生 %1, %2加减分记录?")
                    .arg(studentNo).arg(g2g4Name),
                     QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
        return;
    }

    query1.exec(QString("DELETE FROM gItem "
                           "WHERE studentNo = '%1' and gItem = '%2'").arg(studentNo).arg(g2g4No));

    updategview();

}

void change::on_deleteoButton_clicked()
{
    QModelIndex index = ui->operatorView->currentIndex();
    if (!index.isValid())
        return;

    QSqlRecord record = operatorModel->record(index.row());
    QString g2g4Name = record.value(1).toString();
    QString g2g4No = record.value(0).toString();

    QSqlQuery query1(QString("SELECT * FROM gperator "
                          "WHERE g2g4No = '%1'").arg(g2g4No));

    int r = QMessageBox::warning(this, tr("删除加减分项"),
                    tr("删除加减分编号%1, %2加减分项?")
                    .arg(g2g4No).arg(g2g4Name),
                     QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
            QSqlDatabase::database().rollback();
        return;
    }

    query1.exec(QString("DELETE FROM gOperator "
                           "WHERE g2g4No = '%1'").arg(g2g4No));
    operatorModel->removeRow(index.row());
    operatorModel->submitAll();
    QSqlDatabase::database().commit();

    createoperatorlist();

}

void change::updategview()
{
    updateWhileChangegItem();
    QString studNo = ui->stunumEdit->text();
    gModel->setFilter(QString("studentNo = %1").arg(studNo));
    gModel->setRelation(1,
            QSqlRelation("gOperator", "g2g4No", "g2g4Name"));
    ui->gView->setModel(gModel);
    ui->gView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->gView->setSelectionMode(
             QAbstractItemView::SingleSelection);
    ui->gView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->gView->resizeColumnsToContents();
    ui->gView->setColumnHidden(0, true);
    ui->gView->horizontalHeader()->setStretchLastSection(true);
}

void change::updateWhileChangegItem()
{
    /*start*/
    QString s;
    QSqlQuery queryUpdate;
    s = ui->stunumEdit->text();
    //qDebug() << s;
    QSqlQuery queryG3456Item;
    queryG3456Item.exec(QObject::tr("Select * From gItem where studentNo='%1'").arg(s));

    float studallG3 = 0;
    float studallG4 = 0;
    float studallG5 = 0;
    float studallG6 = 0;
    while (queryG3456Item.next())
    {
        QString c = queryG3456Item.value(1).toString();
        //qDebug() << c;

        float credits = 0;
        QString cname;
        QSqlQuery queryG3Operator;
        queryG3Operator.exec(QObject::tr("Select * from gOperator where g2g4No='%1' and g2g4Attr='g3+'").arg(c));
        while (queryG3Operator.next())
        {
            cname = queryG3Operator.value(1).toString();
            credits = queryG3Operator.value(2).toFloat();
        }
        queryG3Operator.clear();
        studallG3 += credits;
        credits = 0;
        QSqlQuery queryG4Operator;
        queryG4Operator.exec(QObject::tr("Select * from gOperator where g2g4No='%1' and g2g4Attr='g4+'").arg(c));
        while (queryG4Operator.next())
        {
            cname = queryG4Operator.value(1).toString();
            credits = queryG4Operator.value(2).toFloat();
        }
        queryG4Operator.clear();
        studallG4 += credits;
        credits = 0;
        QSqlQuery queryG5Operator;
        queryG5Operator.exec(QObject::tr("Select * from gOperator where g2g4No='%1' and g2g4Attr='g5+'").arg(c));
        while (queryG5Operator.next())
        {
            cname = queryG5Operator.value(1).toString();
            credits = queryG5Operator.value(2).toFloat();
        }
        queryG5Operator.clear();
        studallG5 += credits;
        credits = 0;
        QSqlQuery queryG6Operator;
        queryG6Operator.exec(QObject::tr("Select * from gOperator where g2g4No='%1' and g2g4Attr='g6-'").arg(c));
        while (queryG6Operator.next())
        {
            cname = queryG6Operator.value(1).toString();
            credits = queryG6Operator.value(2).toFloat();
        }
        queryG6Operator.clear();
        studallG6 += credits;
    }
    //qDebug() << studallG3 << studallG4 << studallG5 << studallG6;
    QSqlQuery querySN;
    querySN.exec(QObject::tr("Select * From student where studentNo='%1'").arg(s));
    float G1 = 0, G2 = 0;
    while (querySN.next())
    {
        G1 = querySN.value(2).toFloat();
        G2 = querySN.value(3).toFloat();
    }
    //qDebug() << G1 << G2;
    float G = 0.8 * G1 + 0.1 * G2 + 0.1 * (studallG3 + studallG4 + studallG5) + studallG6;
    //qDebug() << "G=" << G;

    queryUpdate.exec(QObject::tr("update student set G3=%1 where studentNo='%2'").arg(studallG3).arg(s));
    queryUpdate.exec(QObject::tr("update student set G4=%1 where studentNo='%2'").arg(studallG4).arg(s));
    queryUpdate.exec(QObject::tr("update student set G5=%1 where studentNo='%2'").arg(studallG5).arg(s));
    queryUpdate.exec(QObject::tr("update student set G6=%1 where studentNo='%2'").arg(studallG6).arg(s));
    queryUpdate.exec(QObject::tr("update student set G=%1 where studentNo='%2'").arg(G).arg(s));

    /*重新设置studentModel和mapper 方法拷贝如上*/
    updatemapper(s);
}

void change::updatemapper(QString studNo)
{
    ui->g2Edit->setValidator(new QDoubleValidator(this));
    ui->saveg2Button->setEnabled(false);

    QSqlQuery findClassNo;
    findClassNo.exec(QString("Select * from student where studentNo = '%1'").arg(studNo));
    QString classNo = "";
    while (findClassNo.next())
    {
        classNo = findClassNo.value(10).toString();
    }

    studentModel->setTable("student");
    studentModel->setFilter(QString("class='%1'").arg(classNo));
    studentModel->setSort(0, Qt::AscendingOrder);
    studentModel->select();

    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(studentModel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    mapper->addMapping(ui->nameEdit, 1);
    mapper->addMapping(ui->stunumEdit, 0);
    mapper->addMapping(ui->g1Edit,2);
    mapper->addMapping(ui->g2Edit,3);
    mapper->addMapping(ui->g3Edit,4);
    mapper->addMapping(ui->g4Edit,5);
    mapper->addMapping(ui->g5Edit,6);
    mapper->addMapping(ui->g6Edit,7);
    mapper->addMapping(ui->gEdit,8);
    if (studNo != "0000000000") {
        for (int row = 0; row < studentModel->rowCount(); ++row) {
            QSqlRecord record = studentModel->record(row);
            if (record.value(0).toString() == studNo) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }

}

void change::on_g2Edit_textChanged(QString )
{
    ui->saveg2Button->setEnabled(true);
}

void change::on_saveg2Button_clicked()
{
    QString s = ui->stunumEdit->text();
    float g2 = ui->g2Edit->text().toFloat();
    QSqlQuery querysaveg2;
    querysaveg2.exec(QObject::tr("update student set G2=%1 where studentNo='%2'").arg(g2).arg(s));
    querysaveg2.clear();
    ui->saveg2Button->setEnabled(false);
}

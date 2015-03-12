#include "statistics.h"
#include "ui_statistics.h"
#include <QDebug>
#include <QSqlRecord>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlRelationalTableModel>
#include <QtSql/QSqlRelationalDelegate>

statistics::statistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statistics)
{
    ui->setupUi(this);

    widget = new QWidget(this);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(0, 0, 2000, 1414));
    widget->setStyleSheet(QString::fromUtf8("border-image: url(./background.png);"));
    widget->lower();

    staticModel = new QSqlRelationalTableModel(this);
    on_comboBox_3_currentIndexChanged(0);

    studentModel = new QSqlRelationalTableModel(this);
    createStudentModel();
    updateStudentView(Student_No);


    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboBox_3_currentIndexChanged(int)));
    //connect(ui->findButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));
    connect(ui->findcomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkFindEdit()));
    connect(ui->findEdit, SIGNAL(textChanged(QString)), this, SLOT(on_findEdit_textChanged(QString)));
}

statistics::~statistics()
{
    delete ui;
    delete widget;
}

void statistics::createStudentModel()
{
    studentModel->setTable("student");

    studentModel->setHeaderData(Student_No, Qt::Horizontal, tr("ѧ��"));
    studentModel->setHeaderData(Student_name, Qt::Horizontal, tr("����"));
    studentModel->setHeaderData(Student_majLanguage, Qt::Horizontal, tr("��������"));
    studentModel->setHeaderData(Student_major, Qt::Horizontal, tr("רҵ"));
    studentModel->setHeaderData(Student_classNo, Qt::Horizontal, tr("�༶"));
    studentModel->setHeaderData(Student_grade, Qt::Horizontal, tr("�꼶"));
    studentModel->setHeaderData(Student_phone, Qt::Horizontal, tr("�绰"));
    studentModel->setHeaderData(Student_domitory, Qt::Horizontal, tr("����"));
    studentModel->setHeaderData(Student_email, Qt::Horizontal, tr("����"));
    studentModel->setHeaderData(Student_QQ, Qt::Horizontal, tr("QQ��"));
    studentModel->setHeaderData(Student_sex, Qt::Horizontal, tr("�Ա�"));
}

void statistics::updateStudentView(int i)
{
    if (i >= 2 && i <= 8)
        studentModel->setSort(i, Qt::DescendingOrder);
    else
        studentModel->setSort(i, Qt::AscendingOrder);
    if (i == 7)
        studentModel->setSort(i, Qt::AscendingOrder);

    studentModel->select();

    ui->tableView->setModel(studentModel);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->tableView->setSelectionMode(
             QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnHidden(Student_sex, true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void statistics::on_findEdit_textChanged(QString )
{
    ui->findButton->setEnabled(true);
}

void statistics::on_findButton_clicked()
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

                QModelIndex index = ui->tableView->model()->index(row, 0);
                ui->tableView->setCurrentIndex(index);
                ui->tableView->selectRow(index.row());

                //ui->studentView->clicked(index);
                ui->tableView->scrollTo(index);

                break;
            }
        }
        if (iffind == 0)
        {
            QMessageBox::warning(this,tr("��ʾѧ���ɼ�"),
                           tr("�Ҳ�����ѧ��: ѧ��") + studentNo, tr("ȷ��"));
            ui->findEdit->clear();
        }
    } else if (findindex == 0){
        QString studentname = finddes;
        for (int row = 0; row < studentModel->rowCount(); ++row) {
            QSqlRecord record = studentModel->record(row);
            if (record.value(1).toString() == studentname) {
                iffind = 1;
                QModelIndex index = ui->tableView->model()->index(row, 0);
                ui->tableView->setCurrentIndex(index);
                ui->tableView->selectRow(index.row());

                //ui->studentView->clicked(index);
                ui->tableView->scrollTo(index);
                break;
            }
        }
        if (iffind == 0)
        {
            QMessageBox::warning(this,tr("��ʾѧ���ɼ�"),
                           tr("�Ҳ�����ѧ��: ����") + studentname, tr("ȷ��"));
            ui->findEdit->clear();
        }
    }
    studentModel->select();

    ui->tableView->horizontalHeader()->setVisible(
            studentModel->rowCount() > 0);
}

void statistics::checkFindEdit()
{
    int findindex = ui->findcomboBox->currentIndex();
    if (findindex == 1) {
        QRegExp regExp1("[0-9]{0,10}");
        ui->findEdit->setValidator(new QRegExpValidator(regExp1, this));
    } else if (findindex == 0) {
        ui->findEdit->setValidator(0);
    }
}

void statistics::on_radioButton_clicked()
{
    updateStudentView(Student_G1);
}

void statistics::on_radioButton_3_clicked()
{
    updateStudentView(Student_G3);
}

void statistics::on_radioButton_2_clicked()
{
    updateStudentView(Student_G2);
}

void statistics::on_radioButton_4_clicked()
{
    updateStudentView(Student_G4);
}

void statistics::on_radioButton_5_clicked()
{
    updateStudentView(Student_No);
}

void statistics::on_radioButton_6_clicked()
{
    updateStudentView(Student_G);
}

void statistics::on_radioButton_7_clicked()
{
    updateStudentView(Student_G5);
}

void statistics::on_radioButton_8_clicked()
{
    updateStudentView(Student_G6);
}

void statistics::on_comboBox_3_currentIndexChanged(int index)
{
    if (index == Static_major)
    {
        staticModel->setTable("major");
        staticModel->setSort(0, Qt::AscendingOrder);
        staticModel->select();
        ui->comboBox->setModel(staticModel);
    }
    else if (index == Static_grade)
    {
        staticModel->setTable("grade");
        staticModel->setSort(0, Qt::AscendingOrder);
        staticModel->select();
        ui->comboBox->setModel(staticModel);
    }
    else if (index == Static_class)
    {
        staticModel->setTable("class");
        staticModel->setSort(0, Qt::AscendingOrder);
        staticModel->select();
        ui->comboBox->setModel(staticModel);
    }
    else if (index == Static_majLanguage)
    {
        staticModel->setTable("majLanguage");
        staticModel->setSort(0, Qt::AscendingOrder);
        staticModel->select();
        ui->comboBox->setModel(staticModel);
    }

    ui->comboBox->setModelColumn(0);
}

void statistics::on_pushButton_4_clicked()
{
    int index = ui->comboBox_3->currentIndex();
    QString staticDes = ui->comboBox->currentText();
    if (index == Static_class)
    {
         createStudentModel();
         studentModel->setFilter(QString("class = '%1'").arg(staticDes));
    }
    else if (index == Static_major)
    {
        createStudentModel();
        studentModel->setFilter(QString("major = '%1'").arg(staticDes));
    }
    else if (index == Static_majLanguage)
    {
        createStudentModel();
        studentModel->setFilter(QString("majLanguage = '%1'").arg(staticDes));
    }
    else if (index == Static_grade)
    {
        createStudentModel();
        studentModel->setFilter(QString("grade = '%1'").arg(staticDes));
    }
    studentModel->select();
    //ui->tableView->setModel(studentModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void statistics::on_findEdit_returnPressed()
{
    ui->findButton->click();
}

void statistics::on_outputButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
       this,
       QObject::tr("����TXT�ļ����"),
       QDir::currentPath(),
       "Document files (*.txt);;All files(*.*)");
    if (filename.isNull()) {

        // �û�ȡ��ѡ��
        QMessageBox::information(this, QObject::tr("����TXT�ļ����"),
                                 QObject::tr("û��ָ��Ҫ������ļ�Ŀ��"), QMessageBox::Ok | QMessageBox::Cancel);
    } else {
        //�û�ѡ�����ļ�
        QMessageBox::information(this, QObject::tr("����TXT�ļ����"),
                                 QObject::tr("�����ļ��ɹ�"), QMessageBox::Ok | QMessageBox::Cancel);

    // �����ļ�
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))

        return;

    QTextStream out(&file);

    QSqlQuery queryWrite;
    queryWrite.exec(QObject::tr("Select * from student"));

    /*�����ͷ*/
    out << QObject::tr("רҵ\t�༶\tѧ��\t����\tG1\tG2\tG3\tG4\t\G5\tG6\tG\t��ע\n");

    while (queryWrite.next())
    {
        QString stuNo = queryWrite.value(0).toString();
        QString stuName = queryWrite.value(1).toString();
        float g1 = queryWrite.value(2).toFloat();
        float g2 = queryWrite.value(3).toFloat();
        float g3 = queryWrite.value(4).toFloat();
        float g4 = queryWrite.value(5).toFloat();
        float g5 = queryWrite.value(6).toFloat();
        float g6 = queryWrite.value(7).toFloat();
        float comg = queryWrite.value(8).toFloat();
        QString stuMajLan = queryWrite.value(9).toString();
        QString stuClass = queryWrite.value(10).toString();
        QString stuMaj = queryWrite.value(11).toString();
        //QString stuGrade = queryWrite.value(12).toString();

        out << stuMajLan << "\t" << stuMaj << "\t" << stuClass << "\t" << stuNo << "\t" << stuName << "\t"
            << g1 << "\t" << g2 << "\t" << g3 << "\t"
            << g4 << "\t" << g5 << "\t" << g6 << "\t" << comg << "\t";

        QSqlQuery querystugItems;
        QSqlQuery queryg;
        QString gNo;
        QString gName;
        QString temp;
        int itemNumber;
        bool haslast = false;
        bool hasnext = false;
        float gScore;
        /* ���g3item */
        querystugItems.exec(QObject::tr("Select * from gItem where studentNo = '%1'").arg(stuNo));
        while (querystugItems.next())
        {
            gNo = querystugItems.value(1).toString();
            queryg.exec(QObject::tr("Select * from gOperator where g2g4No = '%1' and g2g4Attr='%2'")
                        .arg(gNo).arg("g3+"));
            while (queryg.next()) {
                gName = queryg.value(1).toString();
                gScore = queryg.value(2).toFloat();
                out << gName  << QObject::tr("(")
                    << gScore << QObject::tr("��); ");
                haslast = true;
                //qDebug() << g2g4Name << g2g4Score;
            }
        }
        /* ���g4item */
        hasnext = false;
        temp = "";
        querystugItems.exec(QObject::tr("Select * from gItem where studentNo = '%1'").arg(stuNo));
        while (querystugItems.next())
        {
            gNo = querystugItems.value(1).toString();
            queryg.exec(QObject::tr("Select * from gOperator where g2g4No = '%1' and g2g4Attr='%2'")
                        .arg(gNo).arg("g4+"));
            while (queryg.next()) {
                gName = queryg.value(1).toString();
                gScore = queryg.value(2).toFloat();
                temp += gName  + QObject::tr("(") + QString::number(gScore) + QObject::tr("��); ");
                hasnext = true;
                //qDebug() << g2g4Name << g2g4Score;
            }
        }
        if (hasnext && haslast )
        {
            out << "|";
            haslast = true;
        } else if (hasnext && (!haslast)) {
            haslast = true;
        } else if ((!hasnext) && (!haslast)) {
            haslast = false;
        } else if ((!hasnext) && haslast) {
            haslast = true;
        }
        out << temp;

        /* ���g5item */
        querystugItems.exec(QObject::tr("Select * from gItem where studentNo = '%1'").arg(stuNo));
        hasnext = false;
        temp = "";
        while (querystugItems.next())
        {
            gNo = querystugItems.value(1).toString();
            queryg.exec(QObject::tr("Select * from gOperator where g2g4No = '%1' and g2g4Attr='%2'")
                        .arg(gNo).arg("g5+"));
            while (queryg.next()) {
                gName = queryg.value(1).toString();
                gScore = queryg.value(2).toFloat();
                temp += gName  + QObject::tr("(") + QString::number(gScore) + QObject::tr("��); ");
                hasnext = true;
                //qDebug() << g2g4Name << g2g4Score;
            }
        }
        if (hasnext && haslast )
        {
            out << "|";
            haslast = true;
        } else if (hasnext && (!haslast)) {
            haslast = true;
        } else if ((!hasnext) && (!haslast)) {
            haslast = false;
        } else if ((!hasnext) && haslast) {
            haslast = true;
        }
        out << temp;

        /* ���g6item */
        hasnext = false;
        temp = "";
        querystugItems.exec(QObject::tr("Select * from gItem where studentNo = '%1'").arg(stuNo));
        while (querystugItems.next())
        {
            gNo = querystugItems.value(1).toString();
            queryg.exec(QObject::tr("Select * from gOperator where g2g4No = '%1' and g2g4Attr='%2'")
                        .arg(gNo).arg("g6-"));
            while (queryg.next()) {
                gName = queryg.value(1).toString();
                gScore = queryg.value(2).toFloat();
                temp += gName  + QObject::tr("(") + QString::number(gScore) + QObject::tr("��); ");
                hasnext = true;
                //qDebug() << g2g4Name << g2g4Score;
            }
        }
        if (hasnext && haslast )
        {
            out << "|";
            haslast = true;
        } else if (hasnext && (!haslast)) {
            haslast = true;
        } else if ((!hasnext) && (!haslast)) {
            haslast = false;
        } else if ((!hasnext) && haslast) {
            haslast = true;
        }
        out << temp;

        out << "\n";
    }

    file.close();
    }
}

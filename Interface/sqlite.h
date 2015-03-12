#ifndef SQLITE_H
#define SQLITE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlDriver>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <QFile>
#include <cstdlib>


    float studallG1 = 0,
          studallG2 = 0,
          studallG3 = 0,
          studallG4 = 0,
          studallG5 = 0,
          studallG6 = 0;

    static bool CREATE_SQLITE_CONNECTION()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./student2011NEW.s3db");
        if(!db.open())
        {
            qDebug() << QObject::tr("连接数据库错误");
            QMessageBox::critical(0, QObject::tr("连接数据库错误"), QObject::tr("无法连接到数据库location.s3db，请检查数据库设置！"), QMessageBox::Ok);
            return false;
        }

        return true;
    }


    void readScores()
    {
        QFile inputfile("\li.txt");
        if (!inputfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << QObject::tr("不能打开文件") << "li.txt";
            qDebug() << QObject::tr("程序中止");
            exit(0);
        }
        QString excelID;
        QString studNo;
        QString studName;
        QString courseName;
        QString courseNo;
        float courseScore;
        QString courseCredit;
        QString examDate;
        int examTime;
        QTextStream in(&inputfile);

        if(!CREATE_SQLITE_CONNECTION())
        {
            return;
        }
        QSqlQuery writeg2g4Item;
        writeg2g4Item.exec("DELETE from coursescore");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList list = line.split("\t");
            excelID = list[0];
            studName = list[2];
            courseName = list[4];
            courseScore = list[6].toFloat();
            examDate = list[7];
            courseCredit = list[8];
            if (list[1] != "")
                studNo = list[1];
            if (list[3] != "")
            {
                // 补考学生按第一次考试成绩算，即不算补考成绩
                courseNo = list[3];
                examTime = 1;
                writeg2g4Item.exec(QString("INSERT INTO coursescore values('%1', '%2', %3, '%4', %5)")
                                   .arg(studNo).arg(courseNo).arg(courseScore).arg(examDate).arg(examTime));
            } /*else {
                // 补考学生按补考成绩算
                ++examTime;
                writeg2g4Item.exec(QString("UPDATE coursescore set score = '%1'"
                                           " where studentNo = '%2' and courseNo = '%3'").arg(courseScore).arg(studNo).arg(courseNo));
                writeg2g4Item.exec(QString("UPDATE coursescore set examDate = '%1'"
                                           " where studentNo = '%2' and courseNo = '%3'").arg(examDate).arg(studNo).arg(courseNo));
                writeg2g4Item.exec(QString("UPDATE coursescore set time = '%1'"
                                           " where studentNo = '%2' and courseNo = '%3'").arg(examTime).arg(studNo).arg(courseNo));

            }*/

            qDebug() << studNo << courseNo << courseScore << examDate << examTime;

        }
        inputfile.close();
    }

    void callG()
    {
        if(!CREATE_SQLITE_CONNECTION())
        {
            return;
        }
        QString s;
        QSqlQuery querySN;
        QSqlQuery queryUpdate;
        querySN.exec("Select * From student");
        while (querySN.next())
        {
            s = querySN.value(0).toString();
            float G1 = querySN.value(2).toFloat();
            float G2 = querySN.value(3).toFloat();
            float G3 = querySN.value(4).toFloat();
            float G4 = querySN.value(5).toFloat();
            float G5 = querySN.value(6).toFloat();
            float G6 = querySN.value(7).toFloat();
            qDebug() << s << "G1=" << QString::number(G1)
                        << "G2=" << QString::number(G2)
                        << "G3=" << QString::number(G3)
                        << "G4=" << QString::number(G4)
                        << "G5=" << QString::number(G5)
                        << "G6=" << QString::number(G6);
            queryUpdate.exec(QObject::tr("update student set G=%1 where studentNo='%2'").
                        arg(0.8 * G1 + 0.1 * G2 + 0.1 * (G3 + G4 + G5) + G6).arg(s));
        }
    }

    void callG1()
    {
        if(!CREATE_SQLITE_CONNECTION())
        {
            return;
        }
        QString s;
        QSqlQuery querySN;
        QSqlQuery queryUpdate;
        querySN.exec("Select * From student");
        while (querySN.next())
        {
            studallG1 = 0;
            s = querySN.value(0).toString();
            QSqlQuery queryScore;
            queryScore.exec(QObject::tr("Select * From coursescore where studentNo='%1'").arg(s));


            float studallCre = 0;

            while (queryScore.next())
            {
                QString c = queryScore.value(1).toString();
                float score = queryScore.value(2).toFloat();
                float credits = 0;
                QSqlQuery queryCourse;
                queryCourse.exec(QObject::tr("Select * from course where courseNo='%1'").arg(c));
                QString cname;
                while (queryCourse.next())
                {
                    cname = queryCourse.value(1).toString();
                    credits = queryCourse.value(2).toFloat();
                    studallG1 += score * credits;
                    studallCre += credits;
                }
                queryCourse.clear();

                qDebug() << s << cname << QString::number(credits) << "studallG1=" << QString::number(studallG1);

            }
            queryScore.clear();
            studallG1 = studallG1 / studallCre;

            /*保留两位小数*/
            QString liangweiG1 = QString::number(studallG1, 'f', 2);
            studallG1 = liangweiG1.toFloat();

            qDebug() << s << "G1=" << QString::number(studallG1);
            queryUpdate.exec(QObject::tr("update student set G1=%1 where studentNo ='%2'").arg(studallG1).arg(s));

        }
    }

    void callG3G4G5G6()
    {
        if(!CREATE_SQLITE_CONNECTION())
        {
            return;
        }
        QString s;
        QSqlQuery querySN;
        QSqlQuery queryUpdate;
        querySN.exec("Select * From student");
        while (querySN.next())
        {
            s = querySN.value(0).toString();
            QSqlQuery queryG3456Item;
            queryG3456Item.exec(QObject::tr("Select * From gItem where studentNo='%1'").arg(s));

            studallG3 = 0;
            studallG4 = 0;
            studallG5 = 0;
            studallG6 = 0;
            while (queryG3456Item.next())
            {
                QString c = queryG3456Item.value(1).toString();
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
                qDebug() << s << cname << QString::number(credits) << "studallG6=" << QString::number(studallG6);
            }
            qDebug() << s << "G3=" << QString::number(studallG3)
                     << "G4=" << QString::number(studallG4)
                     << "G5=" << QString::number(studallG5)
                     << "G6=" << QString::number(studallG6);

            queryG3456Item.clear();
            queryUpdate.exec(QObject::tr("update student set G3=%1 where studentNo='%2'").arg(studallG3).arg(s));
            queryUpdate.exec(QObject::tr("update student set G4=%1 where studentNo='%2'").arg(studallG4).arg(s));
            queryUpdate.exec(QObject::tr("update student set G5=%1 where studentNo='%2'").arg(studallG5).arg(s));
            queryUpdate.exec(QObject::tr("update student set G6=%1 where studentNo='%2'").arg(studallG6).arg(s));
        }
    }

    void readG2()
    {
        if(!CREATE_SQLITE_CONNECTION())
        {
            return;
        }
        QString s;
        QSqlQuery querySN;
        QSqlQuery queryUpdate;
        querySN.exec("Select * From student");
        while (querySN.next())
        {
            s = querySN.value(0).toString();
            QSqlQuery queryG2;
            queryG2.exec(QObject::tr("Select * From g2score where studentNo='%1'").arg(s));
            while (queryG2.next())
            {
                studallG2 = queryG2.value(1).toFloat();
            }
            /*保留两位小数*/
            QString liangweiG2 = QString::number(studallG2, 'f', 2);
            studallG2 = liangweiG2.toFloat();
            qDebug() << s << "G2=" << QString::number(studallG2);

            queryG2.clear();
            queryUpdate.exec(QObject::tr("update student set G2=%1 where studentNo='%2'").arg(studallG2).arg(s));
        }
    }

#endif // SQLITE_H

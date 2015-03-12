#ifndef STATISTICS_H
#define STATISTICS_H

#include <QDialog>
#include <QtSql/QSqlRelationalTableModel>

namespace Ui {
    class statistics;
}

class statistics : public QWidget
{
    Q_OBJECT
    enum {
        Student_No = 0,
        Student_name = 1,
        Student_G1 = 2,
        Student_G2 = 3,
        Student_G3 = 4,
        Student_G4 = 5,
        Student_G5 = 6,
        Student_G6 = 7,
        Student_G = 8,

        Student_majLanguage = 9,
        Student_classNo = 10,
        Student_major = 11,
        Student_grade = 12,
        Student_phone = 13,
        Student_domitory = 14,
        Student_email = 15,
        Student_QQ = 16,
        Student_sex = 17
    };

    enum {
        Static_major = 0,
        Static_grade = 1,
        Static_class = 2,
        Static_majLanguage = 3
    };

public:
    explicit statistics(QWidget *parent = 0);
    ~statistics();

private:
    Ui::statistics *ui;
    QWidget * widget;
    QSqlRelationalTableModel *studentModel;
    QSqlRelationalTableModel *staticModel;

public:
    void updateStudentView(int);
    void createStudentModel();

private slots:
    void checkFindEdit();

    void on_findButton_clicked();
    void on_radioButton_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();
    void on_comboBox_3_currentIndexChanged(int index);
    void on_pushButton_4_clicked();
    void on_findEdit_textChanged(QString );
    void on_findEdit_returnPressed();
    void on_outputButton_clicked();
    void on_radioButton_7_clicked();
    void on_radioButton_8_clicked();
};

#endif // STATISTICS_H

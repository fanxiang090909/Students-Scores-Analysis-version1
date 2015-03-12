#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include "QDataWidgetMapper"
#include <QSqlRelationalTableModel>

namespace Ui {
    class Setting;
}

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

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QString id,QWidget *parent = 0);
    void done(int result);

    ~Setting();

private slots:
    void on_saveButton_2_toggled(bool checked);

    void on_phonenumEdit_textEdited(QString );

    void on_emailEdit_textEdited(QString );

    void on_qqEdit_textEdited(QString );

    void on_dormitoryEdit_textEdited(QString );

    void on_saveButton_clicked();

    void on_okButton_clicked();

    void on_emailEdit_returnPressed();

    void on_phonenumEdit_returnPressed();

    void on_qqEdit_returnPressed();

    void on_dormitoryEdit_returnPressed();

private:
    Ui::Setting *ui;

    QWidget * widget;

    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *tableModel;
};

#endif // SETTING_H

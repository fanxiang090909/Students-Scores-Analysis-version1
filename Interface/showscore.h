#ifndef SHOWSCORE_H
#define SHOWSCORE_H
#include "ui_showscore.h"
#include <QDialog>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>

namespace Ui {
    class showscore;
}

enum {
    Course_No = 0,
    Course_name = 1,
    Course_Credits = 2
};

enum {
    Score_studNo = 0,
    Score_courseNo = 1,
    Score_score = 2,
    Score_examDate = 3,
    Score_time = 4

};

class showscore : public QDialog
{
    Q_OBJECT

public:
    explicit showscore(QString studNo, QDialog *parent = 0);
    void createScoreShow(QString studNo);
    void updateG();
    void updategmodel(QString studNo);

    ~showscore();

private slots:
    void on_stunumEdit_textChanged(QString );
    void checkFindEdit();
    void addg();

    void on_findButton_clicked();

    void on_findEdit_textChanged(QString );

    void on_findEdit_returnPressed();

private:
    Ui::showscore *ui;

    QWidget * widget;

    QSqlRelationalTableModel *scoreModel;
    QSqlRelationalTableModel *gModel;
    QSqlRelationalTableModel *tableModel;
    QDataWidgetMapper *mapper;
};

#endif // SHOWSCORE_H

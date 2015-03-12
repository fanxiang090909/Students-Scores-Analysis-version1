#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtSql/QSqlRelationalTableModel>
//#include <QtSql>

//#include <QTableView>


namespace Ui {
    class Window;
}

enum {
    Class_No = 0,
    Class_major = 1,
    Class_grade = 2,
};

enum {
    find_studentname,
    find_studentno,
};

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

signals:
    void statistic();

private slots:
    void updateClassView();
    void updateStudentView();
    void editStudent();
    void checkFindEdit();
    void on_findButton_clicked();
    void clickstatistic();
    void on_logoutButton_clicked();
    void showScore();

    void on_findEdit_returnPressed();

signals:
    void logout();

private:
    Ui::Window *ui;

    QWidget * widget;

    QSqlRelationalTableModel *gradeModel;
    QSqlRelationalTableModel *studentModel;
    QSqlRelationalTableModel *classModel;
    QSqlRelationalTableModel *transModel;

public:
    void createClassView();
    void createStudentView();
};

#endif // WINDOW_H

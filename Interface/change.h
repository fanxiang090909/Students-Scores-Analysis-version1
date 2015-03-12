#ifndef CHANGE_H
#define CHANGE_H

#include <QDialog>
#include <QVector>
#include <QCheckBox>
#include <QButtonGroup>
#include <QScrollArea>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
namespace Ui {
    class change;
}

class change : public QDialog
{
    Q_OBJECT

public:
    explicit change(QString studno, QWidget *parent = 0);
    ~change();

    void createoperatorlist();
    void creategmodel(QString studNo);
    void updateWhileChangegItem();
    void updatemapper(QString studNo);

private slots:
    void addgItem(const QModelIndex &);
    void deletegItem(const QModelIndex &);
    void addgItem2();
    void addgOperator();
    void deleteg();
    void on_deleteoButton_clicked();
    void updategview();

    void on_g2Edit_textChanged(QString );

    void on_saveg2Button_clicked();

private:
    Ui::change *ui;

    QWidget * widget;

    QScrollArea * scroll;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *studentModel;
    QSqlRelationalTableModel *gModel;
    QSqlRelationalTableModel *operatorModel;

};

#endif // CHANGE_H

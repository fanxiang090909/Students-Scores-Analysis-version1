#ifndef ADD_H
#define ADD_H

#include <QDialog>

namespace Ui {
    class add;
}

class add : public QDialog
{
    Q_OBJECT

public:
    explicit add(QWidget *parent = 0);
    ~add();

public slots:
    void setOkButton();

private slots:
    void on_okButton_clicked();

    void on_onumEdit_returnPressed();

private:
    Ui::add *ui;
    QWidget * widget;
};

#endif // ADD_H

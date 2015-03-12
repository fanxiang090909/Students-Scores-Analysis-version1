#ifndef MANAGEWIDGET_H
#define MANAGEWIDGET_H
#include "login.h"
#include "window.h"
#include "setting.h"
#include "about.h"
#include "statistics.h"
#include <QObject>

class ManageWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ManageWidget(QWidget *parent= 0);
    ~ManageWidget();
private:
    Login *lg;
    Window *w;
    About *about;
    statistics *st;

    //Setting *st;
public slots:
    void showLogin();
    void showWindow();
    void showAbout();
    void showStatistics();
    void hideStatistics();
};
#endif // MANAGEWIDGET_H

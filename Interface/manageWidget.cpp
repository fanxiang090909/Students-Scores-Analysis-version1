#include"manageWidget.h"
#include "ui_login.h"

ManageWidget::ManageWidget(QWidget *parent):QWidget(parent)
{
    lg = new Login;
    w = new Window;
    about = new About;
    st = new statistics;

    //设置无边框登录窗口
    lg->setWindowTitle(tr("西北工业大学综合测评系统"));
    lg->setWindowOpacity(1);
    lg->setWindowFlags(Qt::FramelessWindowHint);
    lg->setAttribute(Qt::WA_TranslucentBackground);

    lg->show();

    QObject::connect(lg,SIGNAL(enter()),this,SLOT(showWindow()));
    QObject::connect(w,SIGNAL(logout()),this,SLOT(showLogin()));
    QObject::connect(lg,SIGNAL(help()),this,SLOT(showAbout()));
    QObject::connect(w, SIGNAL(statistic()), this, SLOT(showStatistics()));
}

void ManageWidget::showWindow()
{
    lg->hide();
    w->show();
}
void ManageWidget::showLogin()
{
    w->close();
    delete lg;
    delete w;
    delete about;
    lg = new Login;
    w = new Window;
    about = new About;

    //设置无边框登录窗口
    lg->setWindowTitle(tr("西北工业大学综合测评系统"));
    lg->setWindowOpacity(1);
    lg->setWindowFlags(Qt::FramelessWindowHint);
    lg->setAttribute(Qt::WA_TranslucentBackground);

    lg->show();
    QObject::connect(lg,SIGNAL(enter()),this,SLOT(showWindow()));
    QObject::connect(w,SIGNAL(logout()),this,SLOT(showLogin()));
     QObject::connect(lg,SIGNAL(help()),this,SLOT(showAbout()));
}
ManageWidget::~ManageWidget()
{
    delete lg;
    delete w;
    delete about;
    delete st;
}

void ManageWidget::showAbout()
{
    //设置无边框登录窗口
    about->setWindowTitle(tr("西北工业大学综合测评系统"));
    about->setWindowOpacity(1);
    about->setWindowFlags(Qt::FramelessWindowHint);
    about->setAttribute(Qt::WA_TranslucentBackground);

    about->show();
}

void ManageWidget::showStatistics()
{
    st->show();
}

void ManageWidget::hideStatistics()
{
    st->hide();
}



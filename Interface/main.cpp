#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QTimer>
#include <QSqlError>
#include "login.h"
#include "window.h"
#include "setting.h"
#include "manageWidget.h"
#include "sqlite.h"
#include <QSplashScreen>

/*bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./student2011NEW.s3db");
    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }

    return true;
}*/

int main(int argc, char *argv[])
{
       QApplication a(argc, argv);
       QTextCodec::setCodecForTr(QTextCodec::codecForName("GB18030"));
       QSplashScreen *splash = new QSplashScreen();
       splash->setPixmap(QPixmap("./start.jpg"));
       QTimer::singleShot(3000, splash, SLOT(close()));
       splash->show();
       Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
       splash->showMessage(QObject::tr("正在加载数据..."),topRight,Qt::yellow);

       readScores();
       callG1();
       //readG2();
       //callG3G4G5G6();
       callG();

       if (!CREATE_SQLITE_CONNECTION())
               return 1;       
       splash->showMessage(QObject::tr("正在启动程序..."),topRight,Qt::yellow);

       ManageWidget widget;

       splash->finish(&widget);
       delete splash;

       return a.exec();
}

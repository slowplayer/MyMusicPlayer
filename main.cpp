#include "myplayer.h"
#include <QApplication>
#include <QTextCodec>
#include <QSplashScreen>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("Icon.ico"));
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForLocale(codec);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("screen.png"));
    splash->setSizeIncrement(800,800);
    splash->show();
    for(int i=0;i<200;i++)
    {
       splash->repaint();
    }
    MyPlayer w;
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    splash->finish(&w);
    delete splash;
    return a.exec();
}

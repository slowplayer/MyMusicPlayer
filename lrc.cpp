#include "lrc.h"

Lrc::Lrc(QWidget *parent)
    : QLabel(parent)
{
    duration=0;
    hasLrc=false;
    timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(showLrc()));
}

Lrc::~Lrc()
{

}
void Lrc::addLrcFile(QString file)
{
    filename=file;
    QFile f(filename);
    if(!f.exists())
    {
        setText("没有找到歌词");
        hasLrc=false;
        timer->stop();
    }
    else
    {
        if(!f.open(QFile::ReadOnly|QFile::Text))
        {
            qDebug()<<"无法打开歌词";
            return;
        }
        QTextStream out(&f);
        data=out.readAll();
        qDebug()<<data;
        f.close();
        hasLrc=true;
    }
}
void Lrc::setDuration(qint64 dur)
{
    duration=dur;
}
void Lrc::startLrc()
{
    if(hasLrc)
        timer->start(10);
}
void Lrc::pauseLrc()
{
    if(hasLrc)
        timer->stop();
}
void Lrc::showLrc()
{
    if(hasLrc)
    {
        duration+=10;
        QString tStr;
        QTime currentTime(0,(duration/60000)%60,duration/1000%60,duration%1000);
        QString format="mm:ss.zzz";
        tStr=currentTime.toString(format);
        tStr.chop(1);
        int pa=data.indexOf(tStr);
        if(pa>0)
        {
            QString lrc=data.mid(pa);
            int pb=lrc.indexOf("\n");
            lrc=lrc.left(pb);
            pb=lrc.lastIndexOf("]");
            lrc=lrc.remove(0,pb+1);
            setText(lrc);
        }
    }
    else
    {
        setText("没有找到歌词");
    }
}

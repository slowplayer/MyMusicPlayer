#ifndef LRC_H
#define LRC_H
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QObject>
#include <QDebug>
class Lrc:public QLabel
{
    Q_OBJECT

public:
    Lrc(QWidget *parent=0);
    ~Lrc();
    void addLrcFile(QString file);
    void setDuration(qint64 dur);
    void startLrc();
    void pauseLrc();
private slots:
    void showLrc();
private:
    QString filename;
    QString data;
    QTimer *timer;
    qint64 duration;
    bool hasLrc;
};

#endif // LRC_H

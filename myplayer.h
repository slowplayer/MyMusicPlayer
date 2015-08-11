#ifndef MYPLAYER_H
#define MYPLAYER_H
#include "lrc.h"
#include "httpwindow.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QGridLayout>
#include <QTime>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDomDocument>
#include <QDomNode>
#include <QMap>
class MyPlayer : public QWidget
{
    Q_OBJECT

public:
    MyPlayer(QWidget *parent = 0);
    ~MyPlayer();
private slots:
    void slot_mode(int index); //播放模式变化
    void slot_playTime(qint64 dur); //总时长变化
    void slot_currentTime(qint64 pos); //当前时间变化
    void slot_posSlider(int pos);
    void slot_volumeSlider(int pos);
    void slot_volumeChanged(int volume);
    void slot_run();
    void slot_preSong();
    void slot_nextSong();
    void slot_volumeBt();
    void slot_open();
    void slot_songChanged();
    void slot_viewChanged(QModelIndex index);
    void slot_lrcChanged(int index);

    void query(QNetworkReply *reply);
    QString forLrcid(QString xml);
private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QLabel *nameLab,*volumeLab,*modeLab;
    QLabel *posLab,*durLab;
    QLabel *lrcLab;
    QSlider *posSlider,*volumeSlider;
    QComboBox *modeBox;
    QPushButton *volumeBt;
    QPushButton *runBt,*preBt,*nextBt;
    QListView *listView;
    QStringListModel *model;
    QPushButton *openBt;
    QStringList songNameList;
    Lrc *lrcLabel;

    QNetworkAccessManager *mgr;
    HttpWindow *http;
    QMap<QString,int> song_lrc;
    QString completeName;
};

#endif // MYPLAYER_H

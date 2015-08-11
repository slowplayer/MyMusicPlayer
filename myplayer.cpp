#include "myplayer.h"

MyPlayer::MyPlayer(QWidget *parent)
    : QWidget(parent)
{
    player=new QMediaPlayer(this);
    playlist=new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    http=new HttpWindow;

    nameLab=new QLabel("当前无歌曲");
    lrcLab=new QLabel("歌词:");lrcLabel=new Lrc;
    posLab=new QLabel("00:00");
    posSlider=new QSlider(Qt::Horizontal);
    durLab=new QLabel("00:00");
    preBt=new QPushButton("上一曲");
    runBt=new QPushButton("播放");runBt->setCheckable(true);runBt->setEnabled(false);
    nextBt=new QPushButton("下一曲");
    modeLab=new QLabel("播放模式");
    modeBox=new QComboBox;
    volumeBt=new QPushButton("音量");volumeBt->setCheckable(true);
    volumeSlider=new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0,100);

    mgr=new QNetworkAccessManager(this);

    listView=new QListView;listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model=new QStringListModel;
    listView->setModel(model);
    openBt=new QPushButton("添加本地歌曲");

    modeBox->addItem("单曲播放");
    modeBox->addItem("单曲循环");
    modeBox->addItem("顺序播放");
    modeBox->addItem("循环播放");
    modeBox->addItem("随机播放");
    volumeSlider->setValue(30);

    QGridLayout *grid=new QGridLayout;
    grid->addWidget(nameLab,0,1,1,4);
    grid->addWidget(lrcLab,1,0);grid->addWidget(lrcLabel,1,1,1,3);
    grid->addWidget(posLab,2,0);grid->addWidget(posSlider,2,1,1,3);grid->addWidget(durLab,2,4);
    grid->addWidget(preBt,3,1);grid->addWidget(runBt,3,2);grid->addWidget(nextBt,3,3);
    grid->addWidget(modeLab,4,0);
    grid->addWidget(modeBox,4,1);
    grid->addWidget(volumeBt,4,3);
    grid->addWidget(volumeSlider,4,4);

    QVBoxLayout *vbox=new QVBoxLayout;
    vbox->addWidget(openBt);
    vbox->addWidget(listView);
    //vbox->addWidget(lrcLabel);
    QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addLayout(grid);
    hbox->addLayout(vbox);

    connect(modeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_mode(int)));
    connect(player,SIGNAL(durationChanged(qint64)),this,SLOT(slot_playTime(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(slot_currentTime(qint64)));
    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(slot_volumeChanged(int)));
    connect(posSlider,SIGNAL(sliderMoved(int)),this,SLOT(slot_posSlider(int)));
    connect(volumeSlider,SIGNAL(sliderMoved(int)),this,SLOT(slot_volumeSlider(int)));
    connect(runBt,SIGNAL(clicked()),this,SLOT(slot_run()));
    connect(preBt,SIGNAL(clicked()),this,SLOT(slot_preSong()));
    connect(nextBt,SIGNAL(clicked()),this,SLOT(slot_nextSong()));
    connect(volumeBt,SIGNAL(clicked()),this,SLOT(slot_volumeBt()));
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(query(QNetworkReply*)));
    connect(player,SIGNAL(metaDataChanged()),this,SLOT(slot_songChanged()));
    connect(openBt,SIGNAL(clicked()),this,SLOT(slot_open()));
    connect(listView,SIGNAL(clicked(QModelIndex)),this,SLOT(slot_viewChanged(QModelIndex)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_lrcChanged(int)));

    setLayout(hbox);
    setWindowTitle("MyMusicPlayer");
    setMaximumSize(550,250);
    setMinimumSize(550,250);
}

MyPlayer::~MyPlayer()
{

}
void MyPlayer::slot_mode(int index)
{
    switch (index) {
    case 0:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
        break;
    case 1:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 2:
        playlist->setPlaybackMode(QMediaPlaylist::Sequential);
        break;
    case 3:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 4:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
    default:
        break;
    }
}
void MyPlayer::slot_playTime(qint64 dur)
{

    QTime time(0,dur/60000,dur/1000%60);
    durLab->setText(time.toString("mm:ss"));
    posSlider->setMaximum(dur);
}
void MyPlayer::slot_currentTime(qint64 pos)
{
    QTime time(0,pos/60000,pos/1000%60);
    posLab->setText(time.toString("mm:ss"));
    posSlider->setValue(pos);
    lrcLabel->setDuration(pos);
}
void MyPlayer::slot_posSlider(int pos)
{
    QTime time(0,pos/60000,pos/1000%60);
    posLab->setText(time.toString("mm:ss"));
    player->setPosition(pos);
}
void MyPlayer::slot_volumeSlider(int pos)
{
    player->setVolume(pos);
}
void MyPlayer::slot_volumeChanged(int volume)
{
    player->setVolume(volume);
    volumeSlider->setValue(volume);
}
void MyPlayer::slot_run()
{
    if(runBt->isChecked())
    {
        runBt->setText("暂停");
        lrcLabel->startLrc();
        player->play();
    }
    else
    {
        runBt->setText("播放");
        lrcLabel->pauseLrc();
        player->pause();
    }
}
void MyPlayer::slot_preSong()
{
    playlist->previous();
}
void MyPlayer::slot_nextSong()
{
    playlist->next();
}
void MyPlayer::slot_volumeBt()
{
    if(volumeBt->isChecked())
    {
        volumeBt->setText("静音");
        volumeSlider->setEnabled(false);
        player->setMuted(true);
    }
    else
    {
        volumeBt->setText("音量");
        volumeSlider->setEnabled(true);
        player->setMuted(false);
    }
}
void MyPlayer::slot_open()
{
    QStringList songList=QFileDialog::getOpenFileNames(this,"添加本地歌曲",QDir::homePath(),"歌曲(*.mp3)");
    QFileInfo songInfo;
    if(songList.count()>0)
    {
        foreach (QString songFile,songList)
        {
             playlist->addMedia(QUrl::fromLocalFile(songFile));
             songInfo.setFile(songFile);
             songNameList.append(songInfo.fileName());

        }
        playlist->setCurrentIndex(playlist->mediaCount()-1);
        model->setStringList(songNameList);
        runBt->setEnabled(true);
    }
}
void MyPlayer::slot_songChanged()
{
    QString title=player->metaData(QMediaMetaData::Title).toString();
    QString author=player->metaData(QMediaMetaData::Author).toString();
    completeName=title+"-"+author;
    QString songUrl="http://box.zhangmen.baidu.com/x?op=12&count=1&title="+title+"$$"+author+"$$$$";    
    nameLab->setText(completeName);
    if(song_lrc.value(completeName))
    {
        QString lrcPath="./lrc/"+QString::number(song_lrc.value(completeName))+".lrc";
        lrcLabel->addLrcFile(lrcPath);
    }
    else
    {
        mgr->get(QNetworkRequest(QUrl(songUrl)));
    }
}
void MyPlayer::slot_viewChanged(QModelIndex index)
{
    playlist->setCurrentIndex(index.row());
}
void MyPlayer::slot_lrcChanged(int index)
{
    lrcLabel->setText("");
    lrcLabel->setDuration(0);
}
void MyPlayer::query(QNetworkReply *reply)
{
    QString buffer=reply->readAll();
    int Lrcid=forLrcid(buffer).toInt();
    QString lrcStr="http://box.zhangmen.baidu.com/bdlrc/";
    lrcStr=lrcStr+QString::number(Lrcid/100)+"/"+QString::number(Lrcid)+".lrc";
    song_lrc.insert(completeName,Lrcid);
    if(Lrcid)
    {
        http->downloadFile(lrcStr);
        QString lrcPath="./lrc/"+QString::number(Lrcid)+".lrc";
        lrcLabel->addLrcFile(lrcPath);
    }
}
QString MyPlayer::forLrcid(QString xml)
{
    QString result = "";
    QDomDocument dom;
    dom.setContent(xml);
    QDomElement root=dom.documentElement();
    QDomElement files=root.firstChildElement();
    files=files.nextSiblingElement();
    QDomElement element=files.firstChildElement();
    while(!element.isNull())
    {

        if(element.tagName()=="lrcid")
        {

            result=element.text();
            break;
        }
        element=element.nextSiblingElement();
    }
    return result;
}

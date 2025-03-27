#include "player.h"
#include "challenge_dialog.h"
#include <QCoreApplication>
#include <QDebug>

//when creating a qobject class , qmake should be run.
player::player(QString nameinput,QObject *parent):QObject(parent)
{
    username = nameinput; // set up username

    //the file operations below

    QString basedir = QCoreApplication::applicationDirPath()+"/userdata";
    //base dir is the build/debug , where .exe lay.

    userfolderpath = basedir + "/" +username;

    QDir dir(basedir);
    if(!dir.exists())
    {
        dir.mkpath(".");
    }

    QDir userdir(userfolderpath);
    if(!userdir.exists())
    {
        if(userdir.mkpath("."))
        {
            qDebug() << "success" ;
        }
        else
        {
            qDebug() << "fail";
            return;
        }

    }

    scorelistpath = userfolderpath + "/scorelist.txt";
    challerecordpath = userfolderpath + "/challerecord.txt";

    QFile scorelist(scorelistpath);
    if(!scorelist.exists())
    {
        if(scorelist.open(QIODevice::WriteOnly | QIODevice ::Text))
        {
            scorelist.close();
            qDebug() << "success" ;
        }
        else
        {
            qDebug() << "fail" ;
        }

    }

    QFile challe(challerecordpath);
    if(!challe.exists())
    {
        if(challe.open(QIODevice :: WriteOnly | QIODevice ::Text))
        {

            challe.close();
            qDebug()<<" success "<<challerecordpath;

        }
        else
        {
            qDebug()<<"fail" ;
        }
    }

    //end of the file operations.

    dialogptr = nullptr;

    challenger = nullptr;

}

//void player::updatehighestscore(int time, int step)
//{
//    shortesttime = time;
//    shorteststep = step;
//}



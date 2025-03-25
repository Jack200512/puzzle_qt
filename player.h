#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <QString>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QTextStream>

using namespace std;

class player : public QObject
{
    Q_OBJECT

public:
    QString username;

    QString scorelistpath;

    QString challerecordpath;

    QString userfolderpath;


    bool ischallenged=false;

    player * challenger;

    int shortesttime = -1;
    int shorteststep = -1;



public:
    explicit player(QString,QObject *parent = nullptr);

    void challenged(QString);

    void updatehighestscore(int,int);

    void deleteuser();

    void setupscorelist();


};

#endif // PLAYER_H

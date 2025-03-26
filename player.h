#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <QString>
#include <QObject>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "challenge_dialog.h"

using namespace std;

class player : public QObject
{
    Q_OBJECT

public:
    QString username;

    QString scorelistpath;

    QString challerecordpath;

    QString userfolderpath;


    bool inthebattle = false;

    bool ischallenged = false;

    player * challenger;

    challenge_dialog * dialogptr;


    int challenge_result = 0;

    //0 for lose and 1 for win ,3 for denied.

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

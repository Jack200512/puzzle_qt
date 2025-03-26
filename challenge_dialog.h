#ifndef CHALLENGE_DIALOG_H
#define CHALLENGE_DIALOG_H

#include <QObject>
#include <player.h>
#include "board.h"
#include <vector>

class challenge_dialog : public QObject
{
    Q_OBJECT
    player * challenger;
    player * challengedplr;

    int challenger_time;
    int challenger_step;

    int bechallenged_time;
    int bechallenged_step;

//    int state = 0;
//    //0 waiting , 1 ongoing ,

    vector<vector<int>> theproblem_on;

public:
    challenge_dialog(player*,player*,vector<vector<int>>&,QObject * parent = nullptr);

signals:
    void battleover(player *);
};

#endif // CHALLENGE_DIALOG_H

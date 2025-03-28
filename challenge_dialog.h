#ifndef CHALLENGE_DIALOG_H
#define CHALLENGE_DIALOG_H

#include <QObject>

#include "board.h"
#include <vector>
#include <QString>

class player;

class challenge_dialog : public QObject
{
    Q_OBJECT

public:
    player *challenger;
    player *challengedplr;

    int challenger_time;

    int bechallenged_time;

    int state = -1;
    // 0 for challenger lose,1 for challenger win ,2 for bechanllenged denied,3 for draw;

    QString textforchallenger;
    QString textforbechallenged;

    vector<vector<int>> theproblem_on;

public:
    challenge_dialog(player *, player *, vector<vector<int>> &, QObject *parent = nullptr);

public slots:
    void calculation();

signals:
    void battleover();
};

#endif // CHALLENGE_DIALOG_H

#ifndef FIFPUZZLE_H
#define FIFPUZZLE_H

#include <QMainWindow>
#include "board.h"
#include "QVector"
#include "QPushButton"
#include "QPair"
#include "QButtonGroup"
#include "QString"
#include <vector>
#include <queue>
#include "QTimer"
#include "player.h"


namespace Ui {
class fifpuzzle;
}

class fifpuzzle : public QMainWindow
{
    Q_OBJECT

public:
    explicit fifpuzzle(QWidget *parent = 0);
    ~fifpuzzle();

    board currentboard;

    QTimer * solvetime;

    int timeinsecond;

    int stepcount;

    bool solvedonce = false;

    bool userplaying = false;

    bool clickqueue_optimized = false;


    player * playingplr;

    vector<player*> playercreated;

    vector<pair<int,int>> playingplrscorepairvec;

    vector<pair<player*,int>> userhighestscoretime;

    vector<pair<player*,int>> userhighestscorestep;


    vector<challenge_dialog * > chadialogsum;

private:
    Ui::fifpuzzle *ui;

//    QVector<QPushButton*> qpbgroup;

    QButtonGroup * qbgptr = new QButtonGroup(this);


    void setupscreenwithboard(board&);
    void updatescreen(board &);
    void movefun(int,board&);
    void movefunv2(int);
    int pairtoindex(pair<int,int>);
    pair<int,int> indextopair(int);
    void updateclickablevec();
    void updatechart_show(int);


    vector<int> clickablevec;
    queue<int> clickqueue;
    queue<int> originalcq;
    vector<int> settedsquare;
    vector<int> settedline;

    void updatetheprogressbar(vector<vector<int>>&);
    void updatethestep();

    void dealwithclickqueue();

    void updateperrec();
    void updaterabtirec();
    void updaterabstprec();

    void updatemsgbx();

    void updatechlngerec();

    void updatechlngeinfo();

    void formeruserliserecover();

    void updateusercombo();

    void playerselected(int);


    void updateuserlistbeforecolse();

    void getthescoreofgame();

    void readfromthescorelist();

    void noplayernogame();

    void takeinandupdate_pairvec(pair<int,int>);

    void savescoretotxt();

    void updatehightimevec();

    void takeinandupdate_secondver();

    void updatescorerank_playing_time();

    void updatescorerank_playing_step();





public slots:
    void addtoclickqueue(int);
    void solveandshow();
    void startupdatingui();
    void suffleinwindow();
    void updatesolvetime();
    void startcountingtime();
    void signupuser();

    void playrequest_usersection();

    void challenge_button_click();
    void challen_ac();
    void challen_dn();
};

#endif // FIFPUZZLE_H

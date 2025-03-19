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
    vector<int> settedsquare;
    vector<int> settedline;

    void updatetheprogressbar(vector<vector<int>>&);
    void updatethestep();


public slots:
    void addtoclickqueue(int);
    void solveandshow();
    void startupdatingui();
    void suffleinwindow();
    void updatesolvetime();
    void startcountingtime();
};

#endif // FIFPUZZLE_H

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <utility>
#include <stack>
#include <QObject>
#include <QTimer>
#include <QQueue>

using namespace std;

class board : public QObject
{

    Q_OBJECT
signals:

    void updatesig(int);
    void solvedone();


public:
    explicit board(int size = 4, QObject *parent = nullptr);

    int size;

    int totalstep = 0;

    bool aftersuffle = false;

    vector<vector<int>> chart;

    vector<vector<int>> zeroformchart;


    vector<vector<int>> chartaftersuffle;

    vector<vector<int>> moveablecheck;

    vector<vector<int>> rightposrecord;

    vector<bool> isset;

    pair<int, int> emp_squ_pos;

    pair<int, int> empposinchart2;

    int lastmovedvalue = -1;

    stack<int> suffletrail;

    bool solveandshowclicked = false;



public:
    int dealinglinenum = -1;

    bool solvetenspecialcase = false;

    vector<pair<int, int>> samavavec;

    vector<vector<int>> chartphoto;

//    board();

//    board(int size);

    void solveindumbway();

    void showforinitial();

    void generalshow();

    void showchart(vector<vector<int>> &);

    void suffle();

    int suffletrailreverse(int);

    int getsize()
    {
        return size;
    }

    void movesquare(int);

    void swaptoemp(pair<int, int>);

    void moveemptysquare(int, int step = 1);

    int getvaluebypair(pair<int, int>, vector<vector<int>> &);

    void writebypair(pair<int, int>, int, vector<vector<int>> &);

    void empgotrail(vector<pair<int, int>>);

    //
    //

    vector<pair<int, int>> bypass_x(pair<int, int>, bool);

    vector<pair<int, int>> bypass_x_ver2(pair<int, int>, bool);

    vector<pair<int, int>> bypass_y(pair<int, int>, bool);

    bool checkifblock(vector<pair<int, int>>);

    bool checkifblockforpair(pair<int, int>);

    bool checkifwithin(pair<int, int>);

    int findturingpoint(vector<pair<int, int>>);

    int findturningpoint_yfirst(vector<pair<int, int>>);

    void trailpourin(vector<pair<int, int>>, vector<pair<int, int>> &);

    vector<pair<int, int>> findpathforemp(pair<int, int>);

    vector<pair<int, int>> findpath(pair<int, int>, pair<int, int>);

    vector<pair<int, int>> findpath_yfirst(pair<int, int>, pair<int, int>);

    pair<int, int> generatenew_start(pair<int, int>, bool);

    void foursquarespin(pair<int, int>, int);

    vector<pair<int, int>> findpath_recursion(pair<int, int>, pair<int, int>, vector<pair<int, int>> &);

    vector<pair<int, int>> findpath_stronger(pair<int, int>, pair<int, int>);

    void circletransportforemp(pair<int, int>, pair<int, int>);

    void circlemethod(int);

    void circlediliver(pair<int, int>, pair<int, int>);

    void solve1to8_ver1(int);

    void solve4n8_addition_ver1(int);

    pair<int, int> searchbyvalue(int, const vector<vector<int>> &);

    vector<pair<int, int>> pathgeneratorbypoint(vector<pair<int, int>>);

    vector<pair<int, int>> pathgeneratorbypoint_xversion(vector<pair<int, int>>);

    void solveten();

    void solveeleven();

    void solvetwelve();

    void solverest();

    string textformatter(int);

    //for updating ui while swapping

};

#endif // BOARD_H

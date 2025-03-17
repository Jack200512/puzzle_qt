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

private:
    Ui::fifpuzzle *ui;

//    QVector<QPushButton*> qpbgroup;

    QButtonGroup * qbgptr = new QButtonGroup(this);

    void setupscreenwithboard(board&);
    void updatescreen(board &);
    void movefun(int,board&);
    int pairtoindex(pair<int,int>);
    pair<int,int> indextopair(int);
    void updateclickablevec();

    void solveandshow();

    vector<int> clickablevec;

public slots:
    void solveandshowdisplay(int);
};

#endif // FIFPUZZLE_H

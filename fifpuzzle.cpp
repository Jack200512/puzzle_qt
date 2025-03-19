#include "fifpuzzle.h"
#include "ui_fifpuzzle.h"
#include <QDebug>

fifpuzzle::fifpuzzle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fifpuzzle)
{
    ui->setupUi(this);

    //set all the widget on the screen ready.



    qbgptr->addButton(ui->square1, 0);
    qbgptr->addButton(ui->square2, 1);
    qbgptr->addButton(ui->square3, 2);
    qbgptr->addButton(ui->square4, 3);
    qbgptr->addButton(ui->square5, 4);
    qbgptr->addButton(ui->square6, 5);
    qbgptr->addButton(ui->square7, 6);
    qbgptr->addButton(ui->square8, 7);
    qbgptr->addButton(ui->square9, 8);
    qbgptr->addButton(ui->square10, 9);
    qbgptr->addButton(ui->square11, 10);
    qbgptr->addButton(ui->square12, 11);
    qbgptr->addButton(ui->square13, 12);
    qbgptr->addButton(ui->square14, 13);
    qbgptr->addButton(ui->square15, 14);
    qbgptr->addButton(ui->square16, 15);

    clickablevec.resize(16,0);

    settedsquare.resize(16,0);
    settedline.resize(4,0);

    ui->progressbar->setValue(100);
    setupscreenwithboard(currentboard);

    timeinsecond = 0;
    stepcount = 0;

    solvetime = new QTimer(this);

    connect(solvetime,&QTimer::timeout,this,&fifpuzzle::updatesolvetime);

//    connect(qbgptr, QOverload<int>::of(&QButtonGroup::buttonClicked),this, &fifpuzzle::movefun);
    connect(qbgptr, QOverload<int>::of(&QButtonGroup::buttonClicked),[&](int id) { movefun(id, currentboard); });

    connect(ui->solveandshow,&QPushButton::clicked,this,&fifpuzzle::solveandshow,Qt::UniqueConnection);

    connect(ui->sufflebut,&QPushButton::clicked,this,&fifpuzzle::suffleinwindow),Qt::UniqueConnection;

    connect(&currentboard,&board::updatesig,this,&fifpuzzle::addtoclickqueue);

    connect(&currentboard,&board::solvedone,this,&fifpuzzle::startupdatingui);
    connect(&currentboard,&board::solvedone,this,&fifpuzzle::startcountingtime);

}

void fifpuzzle::setupscreenwithboard(board & boardref)
{
    int currentsetting = 0;
    for(int i= 0;i<4;i++)
    {
        for(int in = 0;in < 4;in++)
        {
            QString text = QString::fromStdString(boardref.textformatter(boardref.chart[i][in]));
            this->qbgptr->button(currentsetting)->setText(text);
            currentsetting++;
        }
    }
}

//void fifpuzzle::updatescreen(board & currentboard)
//{
//    currentboard.emp_squ_pos;
//}

int fifpuzzle::pairtoindex(pair<int, int> pairforpro)
{
    int res;
    res = 4*(pairforpro.second)+pairforpro.first;
    return res;
    //remember the return value is actually the index.
}

pair<int,int> fifpuzzle::indextopair(int index)
{
    pair<int,int> retpair;
    retpair.first = index%4;
    retpair.second = (index-retpair.second)/4;
    return retpair;
}

void fifpuzzle::movefun(int id,board& currentboar)
{

    //update the clickable vector first.
    updateclickablevec();

    int empindex = pairtoindex(currentboar.emp_squ_pos);

    bool commoncase = (id==empindex+1||id == empindex-1||id==empindex - 4||id == empindex+4);


    if(clickablevec[id]==1)
    {
        //gui side
        if(commoncase)
        {
            int empindex = pairtoindex(currentboar.emp_squ_pos);//index of the empty
            pair<int,int> movedsquare = indextopair(id);

            int container = currentboar.getvaluebypair(movedsquare,currentboar.chart);
            qbgptr->button(empindex)->setText(QString::number(container));
            qbgptr->button(id)->setText("   ");
            currentboar.swaptoemp(movedsquare);
        }
        else
        {
            pair<int,int> clickedsquarepos = indextopair(id);

            //find the path in specail case
            vector<pair<int,int>> therail = currentboard.findpath(currentboar.emp_squ_pos,clickedsquarepos);
            for(int i = 0;i<therail.size();i++)
            {
                int empindex = pairtoindex(currentboar.emp_squ_pos);//index of the empty
                pair<int,int> movedsquare = therail[i];

                int container = currentboar.getvaluebypair(movedsquare,currentboar.chart);
                qbgptr->button(empindex)->setText(QString::number(container));
                qbgptr->button(id)->setText("   ");
                currentboar.swaptoemp(movedsquare);
            }

        }
    }

}

void fifpuzzle::updatechart_show(int idinput)
{
    //get the pos with input;
    pair<int,int> pos = indextopair(idinput);
    int value = currentboard.chartaftersuffle[pos.second][pos.first];

    //at the first time, the empposchart2 doesnt need update.

    currentboard.chartaftersuffle[currentboard.empposinchart2.second][currentboard.empposinchart2.first] = value;
    currentboard.chartaftersuffle[pos.second][pos.first] = 0;

    currentboard.empposinchart2 = pos;
}

void fifpuzzle::updateclickablevec()
{
    //clear the former data in the vec.
    clickablevec.assign(clickablevec.size(),0);

    //deal with the square on the same line with the emp square.
    for(int i = 0;i<=3;i++)
    {
        int linenum = currentboard.emp_squ_pos.second;
        int pos = 4*linenum+i;
        clickablevec[pos]=1;
    }

    //deal with the square on the same column with the emp.
    for(int i = 0;i<=3;i++)
    {
        int columnnum = currentboard.emp_squ_pos.first;
        int pos = 4*i+columnnum;
        clickablevec[pos] = 1;
    }
}

void fifpuzzle::solveandshow()
{
    //disable the button while solving.

    currentboard.chartaftersuffle = currentboard.chart;
    currentboard.empposinchart2 = currentboard.emp_squ_pos;

    ui->sufflebut->setEnabled(false);
    ui->solveandshow->setEnabled(false);

    qDebug() << "Button is enabled:" << ui->solveandshow->isEnabled();
    qDebug() << "Button is enabled:" << ui->sufflebut->isEnabled();

    timeinsecond = 0;//every entry to this function causes the clearance of the timeinsecond.

    currentboard.solveandshowclicked = true;
    for (int i = 1; i <= 9; i++)
    {
        qDebug()<<"resolving"<<i;
        currentboard.circlemethod(i);
    }
    currentboard.solveten();
    currentboard.solveeleven();
    currentboard.solvetwelve();
    currentboard.solverest();
    //recover the function of the buttons.
    solvedonce = true;

}

void fifpuzzle::addtoclickqueue(int id)
{
    clickqueue.push(id);
}

void fifpuzzle::startupdatingui()
{


    int length = clickqueue.size();
    if(length ==0) return;

    int idforupdate = clickqueue.front();
    clickqueue.pop();

//    qDebug() << "Setting singleShot timer";
    QTimer::singleShot(500, [=]()
    {
//        qDebug() << "Executing button click for id:" << idforupdate;
        movefunv2(idforupdate);

            // 继续执行下一个点击操作
        startupdatingui();
    });

    if(clickqueue.empty())
    {
        solvetime->stop();

        QTimer::singleShot(3000, this, [=]() {
            ui->sufflebut->setEnabled(true);
            ui->solveandshow->setEnabled(true);
        });

    }

    stepcount++;

    ui->stepcounter->display(stepcount);
}

void fifpuzzle::movefunv2(int id)
{
    pair<int,int> movedsquare = indextopair(id);
    int empindex = pairtoindex(currentboard.empposinchart2);

    int container = currentboard.getvaluebypair(movedsquare,currentboard.chartaftersuffle);
    qbgptr->button(empindex)->setText(QString::number(container));
    qbgptr->button(id)->setText("   ");
    updatechart_show(id);
    updatetheprogressbar(currentboard.chartaftersuffle);
}

void fifpuzzle::updatetheprogressbar(vector<vector<int>>&accordchart)
{
    qDebug()<<"enter updateprogressbar";
    //check how many square are set

    int settedsq = 0;
    int settedli = 0;
    for(int i = 0;i<16;i++)
    {
        pair<int,int> pos = indextopair(i);
        int valuecorrect = currentboard.getvaluebypair(pos,currentboard.rightposrecord);
        int valuereal = currentboard.getvaluebypair(pos,accordchart);

        if(valuecorrect == valuereal)
        {
            settedsquare[i] = 1;
            settedsq++;
        }


    }

    int settedcountinline= 0;
    for(int check = 0;check<13;check+=4)
    {
        //check how many square on the same line is set;
        for(int in = check;in < check+4;in++)
        {
            if(settedsquare[in]==1)
            {
                settedcountinline++;
            }
        }

        if(settedcountinline==4)
        {
            settedline[check/4]= 1;
            settedli++;
        }
        settedcountinline=0;

    }

    int result = settedsq+settedli;

    ui->progressbar->setValue(5*result);

}

void fifpuzzle::suffleinwindow()
{
    currentboard.suffle();
    setupscreenwithboard(currentboard);
    updatetheprogressbar(currentboard.chart);
}

void fifpuzzle::updatesolvetime()
{
    timeinsecond++;
    ui->timecount->display(timeinsecond);
}

void fifpuzzle::startcountingtime()
{
    solvetime->start(1000);
}

fifpuzzle::~fifpuzzle()
{
    delete ui;
}

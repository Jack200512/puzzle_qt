#include "fifpuzzle.h"
#include "ui_fifpuzzle.h"

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

//    connect(qbgptr, QOverload<int>::of(&QButtonGroup::buttonClicked),this, &fifpuzzle::movefun);
    QObject::connect(qbgptr, QOverload<int>::of(&QButtonGroup::buttonClicked),[&](int id) { movefun(id, currentboard); });

    connect(ui->solveandshow,&QPushButton::clicked,this,&fifpuzzle::solveandshow);

//    connect(&currentboard,&board::updatewhilesolving,this,&fifpuzzle::solveandshowdisplay);


    currentboard.suffle();
    setupscreenwithboard(currentboard);




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

void fifpuzzle::updatescreen(board & currentboard)
{
    currentboard.emp_squ_pos;
}

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
    currentboard.solveandshowclicked = true;
    for (int i = 1; i <= 9; i++)
        {
            currentboard.circlemethod(i);
        }
    currentboard.solveten();
    currentboard.solveeleven();
    currentboard.solvetwelve();
    currentboard.solverest();
}

void fifpuzzle::solveandshowdisplay(int id)
{
    qbgptr->button(id)->click();
}

fifpuzzle::~fifpuzzle()
{
    delete ui;
}

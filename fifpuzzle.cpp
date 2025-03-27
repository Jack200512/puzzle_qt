#include "fifpuzzle.h"
#include "ui_fifpuzzle.h"
#include <QDebug>
#include "player.h"
#include "challenge_dialog.h"

fifpuzzle::fifpuzzle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fifpuzzle)
{
    ui->setupUi(this);

    //set all the widget on the screen ready.

    playingplr = nullptr;

    formeruserliserecover();
    updateusercombo();

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

    ui->personalrec->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->rankbytimerec ->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->rankbysteprec -> setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->messagebx->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    connect(qbgptr, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &fifpuzzle::userinterferen);

    connect(ui->solveandshow,&QPushButton::clicked,this,&fifpuzzle::solveandshow,Qt::UniqueConnection);

    connect(ui->sufflebut,&QPushButton::clicked,this,&fifpuzzle::suffleinwindow),Qt::UniqueConnection;


    connect(&currentboard,&board::updatesig,this,&fifpuzzle::addtoclickqueue);

    connect(&currentboard,&board::solvedone,this,&fifpuzzle::startupdatingui);
    connect(&currentboard,&board::solvedone,this,&fifpuzzle::startcountingtime);

    connect(ui->playrequest ,&QPushButton::clicked,this,&fifpuzzle::playrequest_usersection);

    connect(ui->signupconfirm , &QPushButton::clicked,this,&fifpuzzle::signupuser);

    connect(ui->challengereq,&QPushButton::clicked,this,&fifpuzzle::challenge_button_click);

    connect(ui->challen_ac_but,&QPushButton::clicked,this,&fifpuzzle::challen_ac);
    connect(ui->challen_dn_but,&QPushButton::clicked,this,&fifpuzzle::challen_dn);

    connect(ui->hintbutton,&QPushButton::clicked,this,&fifpuzzle::hint);



    noplayernogame();
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


    if(!userplaying)
    {
        startplaying_button();
        startcountingtime();
        userplaying = true;
        stepcount = 0;//clear the stepcount;
        qDebug()<<"goes here";
    }

//    if(hintclicked)
//    {
//        userinterfer = true;
//    }

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
        stepcount++;
        ui->stepcounter->display(stepcount);
    }
    updatetheprogressbar(currentboard.chart);

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
    while(!clickqueue.empty())
    {
        clickqueue.pop();
    }


    clickqueue_optimized = false;

    solvetime ->stop();
    timeinsecond = 0;
    ui->timecount->display(0);

    stepcount = 0;
    ui->stepcounter->display(0);

    userplaying = false;//if solve n show button clicked , it means user is not playing.

    currentboard.moveablecheck = currentboard.zeroformchart;

    currentboard.chartaftersuffle = currentboard.chart;
    currentboard.empposinchart2 = currentboard.emp_squ_pos;

    ui->sufflebut->setEnabled(false);
    ui->solveandshow->setEnabled(false);
    ui->hintbutton->setEnabled(false);
    ui->challengereq->setEnabled(false);
    ui->challen_ac_but->setEnabled(false);
    ui->challen_dn_but->setEnabled(false);
    ui->playrequest->setEnabled(false);
    ui->signupconfirm->setEnabled(false);

    foreach (QAbstractButton *button, qbgptr->buttons()) {
        button->setEnabled(false);

    }


    timeinsecond = 0;//every entry to this function causes the clearance of the timeinsecond.
    stepcount = 0;

    currentboard.solveandshowclicked = true;
    for (int i = 1; i <= 9; i++)
    {

        currentboard.circlemethod(i);
    }
    currentboard.solveten();
    currentboard.solveeleven();
    currentboard.solvetwelve();
    currentboard.solverest();
    //recover the function of the buttons.
    solvedonce = true;

    currentboard.solveandshowclicked = false;

    QString ac = "let me solve this problem!";
    playingplr->messages_boxvec.push_back(ac);
    updatemsgbx();

    //the most important one : update the " record " within in the scope if there is a repeated task.
//    qDebug() << "solve successfully";
}

void fifpuzzle::minisolveandshow()
{
    clickqueue_optimized = false;

   /* userplaying = false;*///if solve n show button clicked , it means user is not playing.

    currentboard.moveablecheck = currentboard.zeroformchart;

    currentboard.chartaftersuffle = currentboard.chart;
    currentboard.empposinchart2 = currentboard.emp_squ_pos;

    currentboard.solveandshowclicked = true;
    for (int i = 1; i <= 9; i++)
    {

        currentboard.circlemethod(i);
    }
    currentboard.solveten();
    currentboard.solveeleven();
    currentboard.solvetwelve();
    currentboard.solverest();
    //recover the function of the buttons.
    solvedonce = true;

    currentboard.solveandshowclicked = false;
}

void fifpuzzle::addtoclickqueue(int id)
{
    clickqueue.push(id);
}

void fifpuzzle::startupdatingui()
{
    //excute once for each solve and show.
    if(!clickqueue_optimized)
    {
        dealwithclickqueue();
    }
    clickqueue_optimized = true;


    if(hintmode)
    {
        hinter = clickqueue.front();
//        movefunv2(clickqueue.front());

        clickqueue.pop();

        hintmode = false;

        ui->sufflebut->setEnabled(true);
        ui->solveandshow->setEnabled(true);
        foreach (QAbstractButton *button, qbgptr->buttons())
        {
            button->setEnabled(true);
        }
        return;
    }


    int length = clickqueue.size();
    if(length ==0) return;

    int idforupdate = clickqueue.front();
    clickqueue.pop();

//    qDebug() << "Setting singleShot timer";
    QTimer::singleShot(300, [=]()
    {
//        qDebug() << "Executing button click for id:" << idforupdate;
        movefunv2(idforupdate);

            // 继续执行下一个点击操作
        startupdatingui();
    });

    if(clickqueue.empty())
    {
        solvetime->stop();

        QString ac = "problem solved!";
        playingplr->messages_boxvec.push_back(ac);
        updatemsgbx();

        QTimer::singleShot(2000, this, [=]() {
            ui->sufflebut->setEnabled(true);
            ui->solveandshow->setEnabled(true);
            ui->hintbutton->setEnabled(true);
            foreach (QAbstractButton *button, qbgptr->buttons())
            {
                button->setEnabled(true);
            }
            asetfinished();
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
//    qDebug()<<"enter updateprogressbar";
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

    if(result==20)//user solved.
    {
        if(userplaying)
        {
            getthescoreofgame();

            updatescorerank_playing_step();
            updatescorerank_playing_time();

            updaterabtirec();
            updaterabstprec();

            if(playingplr->inthebattle)
            {
                //if now playing plr is the challenger,update the challenger record in the dialog;
                if(playingplr == playingplr->dialogptr->challenger)
                {
                    playingplr->dialogptr->challenger_time = ui->timecount->value();
                }
                else
                {
                    playingplr->dialogptr->bechallenged_time = ui->timecount->value();
                    emit bechallenged_done();
                }
            }

            QString ac = "problem solved,congratulations!";
            playingplr->messages_boxvec.push_back(ac);
            updatemsgbx();

            asetfinished();

        }

        updateperrec();
        userplaying = false;//no matter the road it takes to reach the terminal , shift in state;
        solvetime->stop();


    }

}

void fifpuzzle::suffleinwindow()
{
    ui->challengereq->setEnabled(true);
    ui->hintbutton->setEnabled(true);
    ui->solveandshow->setEnabled(true);

    userplaying = false;//suffle and solve both cause the state userplaying to false.
    currentboard.suffle();
    setupscreenwithboard(currentboard);
    updatetheprogressbar(currentboard.chart);

    solvetime->stop();
    ui->timecount->display(0);

    stepcount = 0;
    ui->stepcounter->display(0);

    QString ac = "done with action of suffle.";
    playingplr->messages_boxvec.push_back(ac);
    updatemsgbx();
}

void fifpuzzle::updatesolvetime()
{
    timeinsecond++;
    ui->timecount->display(timeinsecond);
}

void fifpuzzle::startcountingtime()
{
    qDebug()<<"start counting";
    timeinsecond = 0;
    solvetime->start(1000);
}

void fifpuzzle::dealwithclickqueue()
{
    originalcq = clickqueue;
    int length = clickqueue.size();
    //create a vector for inspect.
    vector<int> inspectvector;

    //create a position record vec , clear after finishing the inspect of one num
    vector<int> posrecord;//record the index

    //pour out the int in the queue;
    while(!clickqueue.empty())
    {
        inspectvector.push_back(clickqueue.front());
        clickqueue.pop();
    }

    for(int ispnum = 0;ispnum < 16; ispnum++)
    {
        //from 0 to 15

        for(int searchindex = 0;searchindex<length;searchindex++)
        {
            if(inspectvector[searchindex] == ispnum)
            {
                posrecord.push_back(searchindex);
            }
        }

        //after recording the pos in index,updating the posrecord.
        //start checking the repeated part

        //updating the inspectvector
        if(posrecord.empty())
        {
            posrecord.clear();
            continue;
        }
        for(int collect = 0;collect<posrecord.size()-1;collect++)
        {
            int frontindex = posrecord[collect];
            int backindex = posrecord[collect+1];

            if(frontindex+2 != backindex)
            {
                continue;
            }
            else
            {
                while(inspectvector[frontindex] == inspectvector[backindex])
                {
                    frontindex--;
                    backindex++;
                }
                for(int i = frontindex+1;i<backindex-1;i++)
                {
                    inspectvector[i] = -1;
                }
            }
        }
        posrecord.clear();

    }
    //pouring valid num in inspectvec to queue again
    for(int pour = 0;pour <length;pour++)
    {
        if(inspectvector[pour]!=-1)
        {
            clickqueue.push(inspectvector[pour]);
        }
    }

}

void quickSort(std::vector<std::pair<int, int>> &arr, int left, int right) {
    if (left >= right) return;

    int pivot = arr[left].first;  // 选择最左边的元素作为基准值
    int l = left, r = right;

    while (l < r) {
        // 从右往左找到第一个小于 pivot 的元素
        while (l < r && arr[r].first >= pivot) r--;
        if (l < r) arr[l] = arr[r];  // 移动到左侧

        // 从左往右找到第一个大于 pivot 的元素
        while (l < r && arr[l].first <= pivot) l++;
        if (l < r) arr[r] = arr[l];  // 移动到右侧
    }

    arr[l] = {pivot, arr[left].second};  // 复原基准元素
    quickSort(arr, left, l - 1);   // 递归左侧
    quickSort(arr, l + 1, right);  // 递归右侧
}



void quickSortver2(std::vector<std::pair<int, int>>& vec, int left, int right) {
    if (left >= right) return;

    int pivot = vec[right].second;
    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (vec[j].second < pivot) {
            std::swap(vec[++i], vec[j]);
        }
    }
    std::swap(vec[++i], vec[right]);

    quickSort(vec, left, i - 1);
    quickSort(vec, i + 1, right);
}

void quickSortver2(std::vector<std::pair<player*, int>>& vec, int left, int right) {
    if (left >= right) return;

    int pivot = vec[right].second;
    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (vec[j].second < pivot) {
            std::swap(vec[++i], vec[j]);
        }
    }
    std::swap(vec[++i], vec[right]);

    quickSortver2(vec, left, i - 1);
    quickSortver2(vec, i + 1, right);
}


void fifpuzzle::signupuser()
{


    QString userinp = ui->signname->text();
    if(!userinp.isEmpty()&&userinp.size()<=5)
    {


        player * createplayer = new player(userinp);

//        qDebug()<< "creating user";

        playercreated.push_back(createplayer);

        ui->signname->clear();//remove the input.

        playerselected(playercreated.size()-1);

        QString wlc = "player " + userinp + " created,welcome!";
        playingplr->messages_boxvec.push_back(wlc);

        ui->usercombo->addItem(userinp);

        pair<player* ,int> toadd(createplayer,10000);
        userhighestscoretime.push_back(toadd);
        userhighestscorestep.push_back(toadd);

    }
    else if(userinp.size()>5)
    {
        QString nametolong = "this name is too long!";
        playingplr->messages_boxvec.push_back(nametolong);

    }
    else
    {
        QString alert = "please enter a name!";
        playingplr->messages_boxvec.push_back(alert);
    }

    updatemsgbx();

    noplayernogame();
}


//perrec only interact with pairvec
void fifpuzzle::updateperrec()
{
    ui->personalrec->clear();// to avoid relap
    for(int i = 0;i<playingplrscorepairvec.size();i++)
    {
        QString timestr = QString::number(playingplrscorepairvec[i].first);
        QString stepstr = QString ::number (playingplrscorepairvec[i].second);
        QString scorecontent =  timestr+","+stepstr;
        ui->personalrec->append(scorecontent);
    }

    ui->personalrec->moveCursor(QTextCursor::Start);
}

void fifpuzzle::formeruserliserecover()
{
    QString formeruser = QCoreApplication::applicationDirPath() + "/generaldata/userlist.txt";
    QFile file(formeruser);

    if(!file.open(QIODevice::ReadOnly|QIODevice ::Text))
    {
        qDebug()<< "cannot open" ;
        return;
    }
    if(file.size()==0)
    {
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString singlename = in.readLine().trimmed();
        if(!singlename.isEmpty())
        {
            player * plr = new player(singlename);
            playercreated.push_back(plr);



            playingplr = plr;
            readfromthescorelist();//read user s score in pair form to scorepairvec;

           if(playingplrscorepairvec.empty())
           {
               pair<player*,int> pairforrank(plr,10000);
               pair<player*,int> pairforrank2(plr,10000);

               userhighestscoretime.push_back(pairforrank);
               userhighestscorestep.push_back(pairforrank2);

               updaterabstprec();
               updaterabtirec();
               playingplr = nullptr;
               file.close();
               return;
           }

            //now the pairvec is arranged by time
            int shorttime = playingplrscorepairvec[0].first;

            quickSortver2(playingplrscorepairvec, 0, playingplrscorepairvec.size() - 1);
            //now the pairvec is arranged by step
            int shortstep = playingplrscorepairvec[0].second;

            quickSort(playingplrscorepairvec,0,playingplrscorepairvec.size()-1);

            playingplr = nullptr;

            pair<player*,int> pairforrank(plr,shorttime);
            pair<player*,int> pairforrank2(plr,shortstep);

            userhighestscoretime.push_back(pairforrank);
            userhighestscorestep.push_back(pairforrank2);

        }

    }

    updaterabstprec();
    updaterabtirec();
    file.close();

}


// the update of user combo is only related to the vector.
void fifpuzzle::updateusercombo()
{
    for(int i = 0;i<playercreated.size();i++)
    {
        QString un = playercreated[i]->username;
        ui->usercombo->addItem(un);
    }
}

void fifpuzzle::playrequest_usersection()
{
    int comboindex = ui->usercombo->currentIndex();

    playerselected(comboindex);

    //when user is playing , no other user action is allowed;

    noplayernogame();
}

void fifpuzzle::playerselected(int index)
{
    ui->personalrec->clear();
    savescoretotxt();
    savetochallentxt();
    playingplr = playercreated[index];


    readfromthescorelist();

    ui->playingusername->setText(playingplr->username);


    QString sent = "playing as "+playingplr->username +" now,have fun!\n" + "check if you have a challenge msg!";
    playingplr->messages_boxvec.push_back(sent);
    updatemsgbx();

    if(playingplr->challenger != nullptr)
    {
        QString namedis = playingplr->challenger->username;
        ui->challenger_name->setText(namedis);
    }
    else
    {
        ui->challenger_name->setText("None");
    }


    //to be finish ,update the message box and other stuff.

    updateperrec();

    readfromchallentxt();
    showthebat_res_playing();

    currentboard.chart = currentboard.rightposrecord;
    pair<int,int> pos(3,3);
    currentboard.emp_squ_pos = pos;
    setupscreenwithboard(currentboard);

    userplaying = false;
}

void fifpuzzle::updateuserlistbeforecolse()
{
    QString formeruser = QCoreApplication::applicationDirPath() + "/generaldata/userlist.txt";
    QFile file(formeruser);

    if(!file.open(QIODevice::WriteOnly|QIODevice ::Truncate))
    {
        qDebug()<< "cannot open" ;
        return;
    }

    QTextStream out(&file);

    for(int i = 0;i<playercreated.size();i++)
    {
        QString tobewrite = playercreated[i]->username + "\n";
        out << tobewrite;
    }

    file.close();

}

void fifpuzzle::getthescoreofgame()
{

    //when the player is playing ,there is no need to store the data to the txt
    //when playingplr change,must tansfer the data from pairvec to txt.

    int timeused = ui->timecount->intValue();
    int steps = ui->stepcounter ->intValue();

    pair<int,int> newscorepair(timeused,steps);
    takeinandupdate_pairvec(newscorepair);

}

//read from playingplr score txt to the pairvec.
void fifpuzzle::readfromthescorelist()//only read if the playing player is not empty.
{
    playingplrscorepairvec.clear();
    //read and create a the vec of step and time.
    QString path = playingplr -> scorelistpath;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return;
    }

    if(file.size()==0)
    {
        file.close();
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty())
        {
            continue;
        }

        QStringList datalist = line.split(",");



        QString timescore = datalist[0].trimmed();
        QString stepscore = datalist[1].trimmed();

        int timenum = timescore.toInt();
        int stepnum = stepscore.toInt();

        pair<int,int> scorepair(timenum,stepnum);
        takeinandupdate_pairvec(scorepair);


    }
    file.close();
}

void fifpuzzle::noplayernogame()
{
    if(playingplr == nullptr)
    {
        foreach (QAbstractButton *button, qbgptr->buttons()) {
            button->setEnabled(false);

        }
        ui->sufflebut->setEnabled(false);
        ui->solveandshow->setEnabled(false);
        ui->hintbutton->setEnabled(false);
    }
    else
    {
        foreach (QAbstractButton *button, qbgptr->buttons()) {
            button->setEnabled(true);

        }
        ui->sufflebut->setEnabled(true);
        ui->solveandshow->setEnabled(true);
        ui->hintbutton->setEnabled(true);
    }
}

void fifpuzzle::takeinandupdate_pairvec(pair<int,int> toaddpair)
{

    playingplrscorepairvec.push_back(toaddpair);
    quickSort(playingplrscorepairvec, 0, playingplrscorepairvec.size() - 1);

//    pair<int,int> highintime = playingplrscorepairvec[0];
//    playingplr->updatehighestscore(highintime.first,highintime.second);

}

void fifpuzzle::savescoretotxt()
{
    if(playingplr==nullptr)
    {
        return;
    }
    QString path = playingplr->scorelistpath;
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        return;
    }

    QTextStream out(&file);

    for(int i = 0;i<playingplrscorepairvec.size();i++)
    {
        pair<int,int> current = playingplrscorepairvec[i];
        QString text = QString::number(current.first) + "," +QString::number(current.second)+"\n";
        out << text;
    }

    file.close();


}

void fifpuzzle::updaterabtirec()
{
    ui->rankbytimerec->clear();
    quickSortver2(userhighestscoretime, 0, userhighestscoretime.size() - 1);
    for(int i = 0 ;i<userhighestscoretime.size();i++)
    {
        if(userhighestscoretime[i].second == 10000)
        {
            continue;//no score yet;
        }
        QString name = userhighestscoretime[i].first->username +" "+ QString::number(userhighestscoretime[i].second);
        ui->rankbytimerec->append(name);

    }
    ui->rankbytimerec->moveCursor(QTextCursor::Start);
}


void fifpuzzle::updaterabstprec()
{
    ui->rankbysteprec->clear();
    quickSortver2(userhighestscorestep,0,userhighestscorestep.size()-1);
    for(int i = 0 ;i<userhighestscorestep.size();i++)
    {
        if(userhighestscorestep[i].second == 10000)
        {
            continue;//no score yet;
        }
        QString name = userhighestscorestep[i].first->username+" "+ QString::number(userhighestscorestep[i].second);

        ui->rankbysteprec->append(name);

    }
    ui->rankbysteprec->moveCursor(QTextCursor::Start);

}

void fifpuzzle::updatescorerank_playing_time()
{
    int index =0;
    while(userhighestscoretime[index].first != playingplr)
    {
        index++;
    }

    int currentvalue = userhighestscoretime[index].second;
    int newscore = ui->timecount->value();

    //only update when a new highest score come out.
    if(newscore < currentvalue)
    {
        userhighestscoretime[index].second = newscore;
    }

}

void fifpuzzle::updatescorerank_playing_step()
{
    int index = 0;
    while(userhighestscorestep[index].first != playingplr)
    {
        index++;
    }

    int currentstepval = userhighestscorestep[index].second;
    int newscore = ui->stepcounter->value();

    if(newscore< currentstepval)
    {
        userhighestscorestep[index].second = newscore;
    }
}




void fifpuzzle::challenge_button_click()
{
    if(playingplr->dialogptr != nullptr)
    {
        return;
    }

    int plrindex = ui->usercombo->currentIndex();
    player * plp = playercreated[plrindex];

    if(plp == playingplr)
    {
        return;
    }

    //get the challenged player ptr;

    //get the currentboard chart;
    vector<vector<int>> currentchart = currentboard.chart;


    challenge_dialog * challedia = new challenge_dialog(playingplr,plp,currentchart);

    chadialogsum.push_back(challedia);

    playingplr->dialogptr = challedia;
    plp->dialogptr = challedia;

    playingplr->inthebattle = true;
    plp->challenger = playingplr;

    connect(this,&fifpuzzle::bechallenged_done,&*(playingplr->dialogptr),&challenge_dialog::calculation);
    connect(&*(playingplr->dialogptr),&challenge_dialog::battleover,this,&fifpuzzle::showthebat_res_playing);

    ui->challengereq->setEnabled(false);
    ui->sufflebut->setEnabled(false);//if challengeo other player,you can not skip away;
    ui->playrequest->setEnabled(false);
    ui->signupconfirm->setEnabled(false);

    QString msg = "challenge msg sent to "+plp->username+",wait for response!";
    playingplr->messages_boxvec.push_back(msg);
    updatemsgbx();

    QString msg2 = "a new challenge request from "+playingplr->username+" came in,check it up!";
    plp->messages_boxvec.push_back(msg2);
}

void fifpuzzle::challen_ac()
{
    ui->sufflebut->setEnabled(false);
    qDebug()<<"challen accepted";
    if(playingplr->dialogptr == nullptr)
    {
        return;
    }

    //only bechallenged plr can accept the challenge.
    bool bechallenger_ac = playingplr->dialogptr->challengedplr == playingplr;

    if(bechallenger_ac)
    {
        QString ac = "challenge accepted!";
        playingplr->messages_boxvec.push_back(ac);
        updatemsgbx();


        playingplr->inthebattle = true;
        currentboard.chart = playingplr->dialogptr->theproblem_on;
        currentboard.emp_squ_pos = currentboard.searchbyvalue(0,currentboard.chart);

        setupscreenwithboard(currentboard);

        ui->challenger_name->setText("None");

    }
}


void fifpuzzle::showthebat_res_playing()
{
    //if in the state of battle, then enter the judgement phase;
    if(playingplr->dialogptr != nullptr)
    {
        if(playingplr->dialogptr->state != -1)
        {
            playingplr->dialogptr->challenger->dialogptr = nullptr;//release the dialogptr
            delete playingplr->dialogptr;
            playingplr -> dialogptr = nullptr;
        }
    }


    ui->challen_rec->clear();
    for(int i= 0;i<playingplr->challenge_msg.size();i++)
    {
        ui->challen_rec->append(playingplr->challenge_msg[i]);
    }
    ui->challen_rec->moveCursor(QTextCursor::Start);
}

void fifpuzzle::challen_dn()
{
    if(playingplr->dialogptr == nullptr)
    {
        return;
    }

    QString textforchallenger = "vs." + playingplr->dialogptr->challengedplr->username + " == " +"Denied";
    QString textforbechallenged = "vs." + playingplr->dialogptr->challenger->username + " == " + "Deny";

    playingplr->challenge_msg.push_back(textforbechallenged);
    playingplr->dialogptr->challenger->challenge_msg.push_back(textforchallenger);

    playingplr->dialogptr->state = 2;
    showthebat_res_playing();

    QString ac = "challenge denied!";
    playingplr->messages_boxvec.push_back(ac);
    updatemsgbx();
}

void fifpuzzle::readfromchallentxt()
{
    QString path = playingplr->challerecordpath;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"wrong";
        return;
    }

    if(file.size() == 0)
    {
        file.close();
        return;
    }

    QTextStream in(&file);

   playingplr->challenge_msg.clear();
    while(!in.atEnd())
    {
        QString singleline = in.readLine();
        playingplr->challenge_msg.push_back(singleline);
    }

    file.close();
}

void fifpuzzle::savetochallentxt()
{
    if(playingplr==nullptr)
    {
        return;
    }
    QString path = playingplr->challerecordpath;
    QFile file(path);

    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        qDebug()<<"wrong";
        return;
    }

    QTextStream out(&file);

    for(int i = 0;i<playingplr->challenge_msg.size();i++)
    {
        out << playingplr->challenge_msg[i]+"\n";
    }
    file.close();
}

void fifpuzzle::hint()
{

    hintclicked = true;

    connect(ui->sufflebut,&QPushButton::clicked,this,&fifpuzzle::userinterferen);

    disconnect(&currentboard,&board::solvedone,this,&fifpuzzle::startcountingtime);

    QString ac = "let me give you a hand,come on!";
    playingplr->messages_boxvec.push_back(ac);
    updatemsgbx();


    vector<vector<int>> photo = currentboard.chart;
    pair<int,int> emppho = currentboard.emp_squ_pos;


    if(clickqueue.empty()||userinterfer)
    {
        hintmode =true;
        while(!clickqueue.empty())
        {
            clickqueue.pop();
        }
        minisolveandshow();
        userinterfer = false;
    }
    else
    {
        hinter = clickqueue.front();
        clickqueue.pop();
    }


    currentboard.chart = photo;
    currentboard.emp_squ_pos = emppho;

    movefun(hinter,currentboard);

    userplaying = true;

    connect(&currentboard,&board::solvedone,this,&fifpuzzle::startcountingtime);


}

void fifpuzzle::userinterferen()
{
    userinterfer = true;
}

void fifpuzzle::startplaying_button()
{
    ui->playrequest->setEnabled(false);
    ui->signupconfirm -> setEnabled(false);
    ui->challen_ac_but->setEnabled(false);
    ui->challen_dn_but -> setEnabled(false);
    ui->challengereq->setEnabled(true);

    ui->solveandshow->setEnabled(true);
    ui->hintbutton ->setEnabled(true);
}

void fifpuzzle::asetfinished()
{
    ui->playrequest->setEnabled(true);
    ui->signupconfirm -> setEnabled(true);
    ui->challen_ac_but->setEnabled(true);
    ui->challen_dn_but -> setEnabled(true);
    ui->challengereq->setEnabled(false);

    ui->solveandshow->setEnabled(false);
    ui->hintbutton ->setEnabled(false);
}

void fifpuzzle::updatemsgbx()
{
    ui->messagebx->setFontItalic(true);
    ui->messagebx->clear();
    for(int i = 0; i<playingplr->messages_boxvec.size();i++)
    {
        if(i == playingplr->messages_boxvec.size()-1)
        {
            ui->messagebx->setTextColor(Qt::red);
        }

        ui->messagebx->append("***"+playingplr->messages_boxvec[i]);
        ui->messagebx->setTextColor(Qt::black);

    }
}


fifpuzzle::~fifpuzzle()
{
    savescoretotxt();
    updateuserlistbeforecolse();
    delete ui;
}

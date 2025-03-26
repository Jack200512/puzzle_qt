#include "fifpuzzle.h"
#include "ui_fifpuzzle.h"
#include <QDebug>
#include "player.h"

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
        ui->playrequest->setEnabled(false);
        ui->challengereq->setEnabled(false);
        ui->signupconfirm->setEnabled(false);
        startcountingtime();
        userplaying = true;
        stepcount = 0;//clear the stepcount;
    }
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

    foreach (QAbstractButton *button, qbgptr->buttons()) {
        button->setEnabled(false);

    }

//    qDebug() << "Button is enabled:" << ui->solveandshow->isEnabled();
//    qDebug() << "Button is enabled:" << ui->sufflebut->isEnabled();

    timeinsecond = 0;//every entry to this function causes the clearance of the timeinsecond.
    stepcount = 0;

    currentboard.solveandshowclicked = true;
    for (int i = 1; i <= 9; i++)
    {
//        qDebug()<<"resolving"<<i;
        currentboard.circlemethod(i);
    }
    currentboard.solveten();
    currentboard.solveeleven();
    currentboard.solvetwelve();
    currentboard.solverest();
    //recover the function of the buttons.
    solvedonce = true;

    currentboard.solveandshowclicked = false;
    //the most important one : update the " record " within in the scope if there is a repeated task.
    qDebug() << "solve successfully";
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

        QTimer::singleShot(2000, this, [=]() {
            ui->sufflebut->setEnabled(true);
            ui->solveandshow->setEnabled(true);
            foreach (QAbstractButton *button, qbgptr->buttons())
            {
                button->setEnabled(true);
            }
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

    if(result==20)
    {
        if(userplaying)
        {
            getthescoreofgame();

            updatescorerank_playing_step();
            updatescorerank_playing_time();

            updaterabtirec();
            updaterabstprec();

            ui->playrequest->setEnabled(true);
            ui->challengereq->setEnabled(true);
            ui->signupconfirm->setEnabled(true);
        }
        updateperrec();
        userplaying = false;//no matter the road it takes to reach the terminal , shift in state;
        solvetime->stop();


    }

}

void fifpuzzle::suffleinwindow()
{
    userplaying = false;//suffle and solve both cause the state userplaying to false.
    currentboard.suffle();
    setupscreenwithboard(currentboard);
    updatetheprogressbar(currentboard.chart);

    solvetime->stop();
    ui->timecount->display(0);

    stepcount = 0;
    ui->stepcounter->display(0);
}

void fifpuzzle::updatesolvetime()
{
    timeinsecond++;
    ui->timecount->display(timeinsecond);
}

void fifpuzzle::startcountingtime()
{
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
    if(!userinp.isEmpty())
    {
        player * createplayer = new player(userinp);

        qDebug()<< "creating user";

        playercreated.push_back(createplayer);

        ui->signname->clear();//remove the input.

        playerselected(playercreated.size()-1);

        ui->usercombo->addItem(userinp);

        pair<player* ,int> toadd(createplayer,10000);
        userhighestscoretime.push_back(toadd);
        userhighestscorestep.push_back(toadd);

    }
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
    playingplr = playercreated[index];

    readfromthescorelist();

    ui->playingusername->setText(playingplr->username);


    //to be finish ,update the message box and other stuff.

    updateperrec();


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
    }
    else
    {
        foreach (QAbstractButton *button, qbgptr->buttons()) {
            button->setEnabled(true);

        }
        ui->sufflebut->setEnabled(true);
        ui->solveandshow->setEnabled(true);
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

}

//void fifpuzzle::challen_ac()
//{
//    if(playingplr->dialogptr == nullptr)
//    {
//        return;
//    }

//    playingplr->
//}


fifpuzzle::~fifpuzzle()
{
    savescoretotxt();
    updateuserlistbeforecolse();
    delete ui;
}

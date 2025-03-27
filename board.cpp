#include "board.h"
#include <random>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <chrono>

#include <QThread>

bool pairisinthegroup(pair<int,int>,vector<pair<int,int>>);

int randomchoose()
{
    std::srand(std::time(0));
    int randomInRange = std::rand() % 4;
    return randomInRange;
}

int getRandomInt()
{
    static std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, 3);

    return dist(gen); // 生成并返回随机数
}



board::board(int size, QObject *parent) : QObject(parent), size(size)
{
    chart.resize(size);
    for (int iforvec = 0; iforvec < size; iforvec++)
    {
        chart[iforvec].resize(size);
    }

    int writenum = 1;
    for (int i = 0; i < size; i++)
    {
        for (int ini = 0; ini < size; ini++)
        {
            chart[i][ini] = writenum;
            writenum++;
        }
    }

    chart[3][3] = 0;

    moveablecheck = chart;

    rightposrecord = chart;



    for (auto &row : moveablecheck)
    {
        std::fill(row.begin(), row.end(), 1);
    }

    zeroformchart = moveablecheck;

    emp_squ_pos.first = 3;
    emp_squ_pos.second = 3;

}

void board::showforinitial()
{
    int counter = 9;
    for (int i = 0; i < 4; i++)
    {
        for (int ini = 0; ini < 4; ini++)
        {
            if (counter > 0)
            {
                //cout << "0" << chart[i][ini] << " ";
            }
            else
            {
                //cout << chart[i][ini] << " ";
            }
            counter--;
        }

        //cout << endl;
    }
}

void board::generalshow()
{
    //cout << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int in = 0; in < 4; in++)
        {
            if (chart[i][in] == 0)
            {
                //cout << "  " << " ";
            }
            else if (chart[i][in] < 10)
            {
                //cout << "0" << chart[i][in] << " ";
            }
            else
            {
                //cout << chart[i][in] << " ";
            }
        }

        //cout << endl;
    }
}

string board::textformatter(int input)
{
    string forret;
    if (input == 0)
    {
        return forret = "   ";
    }
    else
    {
        return forret = to_string(input);
    }
}

void board::showchart(vector<vector<int>> &showch)
{
    //cout << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int in = 0; in < 4; in++)
        {
            if (showch[i][in] == 0)
            {
                //cout << "  " << " ";
            }
            else if (showch[i][in] < 10)
            {
                //cout << "0" << showch[i][in] << " ";
            }
            else
            {
                //cout << showch[i][in] << " ";
            }
        }

        //cout << endl;
    }
}

void board::swaptoemp(pair<int, int> forswap)
{
    int valuecontainer;
    valuecontainer = getvaluebypair(forswap, chart);

    writebypair(forswap, 0, chart);
    writebypair(emp_squ_pos, valuecontainer, chart);

    emp_squ_pos = forswap;

    if (aftersuffle)
    {
        totalstep++;
    }

    if(solveandshowclicked)
    {
        int id = forswap.second*4 + forswap.first;
        emit updatesig(id);
    }

}

void board::movesquare(int direction)
{
    int movex = emp_squ_pos.first;
    int movey = emp_squ_pos.second;

    int valuecontainer = -1;

    pair<int, int> formovepair;

    switch (direction)
    {
    case 0:
        formovepair.first = movex;
        formovepair.second = movey - 1;
        break;
    case 1:
        formovepair.first = movex + 1;
        formovepair.second = movey;
        break;

    case 2:
        formovepair.first = movex;
        formovepair.second = movey + 1;
        break;

    case 3:
        formovepair.first = movex - 1;
        formovepair.second = movey;
        break;

    default:
        break;
    }

    swaptoemp(formovepair);
}

void board::moveemptysquare(int direct, int step)
{
    while (step)
    {
        movesquare(direct);
        step--;
    }
}

// the range really matter.
int board::getvaluebypair(pair<int, int> pos, vector<vector<int>> &samplechart)
{
    return samplechart[pos.second][pos.first];
}

void board::writebypair(pair<int, int> writepos, int value, vector<vector<int>> &samplechart)
{
    samplechart[writepos.second][writepos.first] = value;
}

void board::suffle()
{
    int step = 10000;

    // evaluate the ava number through the postition of empty square

    // get ava
    while (step)
    {
        int inspect_x = emp_squ_pos.first;
        int inspect_y = emp_squ_pos.second;

        vector<pair<int, int>> avavec;

        vector<pair<int, int>> pairforinspectvec;

        // assume the 4 point around is valid.

        pair<int, int> pforcheck1(inspect_x, inspect_y - 1);
        pair<int, int> pforcheck2(inspect_x + 1, inspect_y);
        pair<int, int> pforcheck3(inspect_x, inspect_y + 1);
        pair<int, int> pforcheck4(inspect_x - 1, inspect_y);

        pairforinspectvec.push_back(pforcheck1);
        pairforinspectvec.push_back(pforcheck2);
        pairforinspectvec.push_back(pforcheck3);
        pairforinspectvec.push_back(pforcheck4);

        // inspect the inspectvec.
        for (int i = 0; i < 4; i++)
        {
            // check x
            bool insx = (pairforinspectvec[i].first <= 3) && (pairforinspectvec[i].first >= 0);
            // check y
            bool insy = (pairforinspectvec[i].second <= 3) && (pairforinspectvec[i].second >= 0);

            if (insx && insy)
            {
                avavec.push_back(pairforinspectvec[i]);
            }
            else
            {
                pairforinspectvec[i].first = -1;
                pairforinspectvec[i].second = -1;

                avavec.push_back(pairforinspectvec[i]);
            }
        }

        // save the avaliable pair in clockwise range.

        samavavec = avavec; // testing sentence.

        int rannum;

        // get the right random number, no repeat.
        while (1)
        {
            rannum = getRandomInt();
            // if we pick up the invalid pair or the last moved pair ,reselect.
            if (avavec[rannum].first != -1 && getvaluebypair(avavec[rannum], chart) != lastmovedvalue)
            {
                break;
            }
        }

        pair<int, int> selectedpair = avavec[rannum];

        int valuestore = getvaluebypair(selectedpair, chart);

        writebypair(emp_squ_pos, valuestore, chart);

        writebypair(selectedpair, 0, chart);

        emp_squ_pos = selectedpair;

        lastmovedvalue = valuestore;

        suffletrail.push(suffletrailreverse(rannum));

        step--;

        avavec.clear();

        chartphoto = chart; // take a photo after suffle;

        aftersuffle = true;

        chartaftersuffle = chart;

        empposinchart2 = emp_squ_pos;
        // generalshow();
        // ////cout << endl;
    }
}

int board::suffletrailreverse(int chose)
{
    switch (chose)
    {
    case 0:
        return 2;

    case 1:
        return 3;

    case 2:
        return 0;

    case 3:
        return 1;

    default:
        return -1;
    }
}

void board::solveindumbway()
{
    while (!suffletrail.empty())
    {
        movesquare(suffletrail.top());
        suffletrail.pop();
        generalshow();
        //cout << endl;
    }
}

pair<int, int> board::searchbyvalue(int val, const vector<vector<int>> &searchingchart)
{
    pair<int, int> pcontainer;
    for (int i = 0; i < size; i++)
    {
        for (int ini = 0; ini < size; ini++)
        {
            if (searchingchart[i][ini] == val)
            {
                pcontainer.first = ini;
                pcontainer.second = i;
                return pcontainer;
            }
        }
    }
    pcontainer.first = -1;
    pcontainer.second = 0;
    return pcontainer;
}

void board::empgotrail(vector<pair<int, int>> theemptrail)
{
    for (int i = 0; i < theemptrail.size(); i++)
    {
        swaptoemp(theemptrail[i]);
        generalshow();
    }
}

bool board::checkifwithin(pair<int, int> posforcheck)
{
    if (posforcheck.first < 0 || posforcheck.first > 3)
    {
        return false;
    }

    if (posforcheck.second < 0 || posforcheck.second > 3)
    {
        return false;
    }

    return true;
}

bool board::checkifblock(vector<pair<int, int>> trailforcheck)
{
    for (int i = 0; i < trailforcheck.size(); i++)
    {
        bool iswithin = checkifwithin(trailforcheck[i]);

        if (!iswithin)
        {
            return true;
        }

        int valueinchart = getvaluebypair(trailforcheck[i], chart);
        pair<int, int> coorpos = searchbyvalue(valueinchart, rightposrecord);
        bool moveable = getvaluebypair(coorpos, moveablecheck);

        if (!moveable)
        {
            return true;
        }
    }

    return false;
}

bool board::checkifblockforpair(pair<int, int> pforcheck)
{
    // get value in chart by pos in chart.
    int valueinchart = getvaluebypair(pforcheck, chart);

    // get the pos in rightposrecord by the value in chart.
    pair<int, int> coorpos = searchbyvalue(valueinchart, rightposrecord);

    // get the state with the pos in rightposrecord.
    bool moveable = getvaluebypair(coorpos, moveablecheck);

    if (!moveable)
    {
        return true;
    }

    else
    {
        return false;
    }
}

vector<pair<int, int>> board::bypass_x(pair<int, int> start, bool toright)
{
    vector<pair<int, int>> bypasstrail1;
    vector<pair<int, int>> bypasstrail2;
    bypasstrail1.resize(3);
    bypasstrail2.resize(3);

    pair<int, int> start1(start.first, start.second - 1);
    pair<int, int> start2(start.first, start.second + 1);

    // default to right.
    pair<int, int> des1(start1.first + 2, start1.second);
    pair<int, int> des2(start2.first + 2, start2.second);

    if (!toright)
    {
        des1.first -= 4;
        des2.first -= 4;
    }

    bypasstrail1 = findpath(start1, des1);
    bypasstrail2 = findpath(start2, des2);

    // fatal error if both of the trails has without point,may reture the wrong one with right one ignored.

    bool t1blocked = checkifblock(bypasstrail1);

    if (t1blocked)
    {
        return bypasstrail2;
    }
    else
    {
        return bypasstrail1;
    }
}

vector<pair<int, int>> board::bypass_y(pair<int, int> start, bool up)
{
    vector<pair<int, int>> bypasstrail1;
    vector<pair<int, int>> bypasstrail2;

    pair<int, int> start1(start.first - 1, start.second);
    pair<int, int> start2(start.first + 1, start.second);

    pair<int, int> des1(start1.first, start1.second - 2);
    pair<int, int> des2(start2.first, start2.second - 2);

    if (!up)
    {
        des1.second += 4;
        des2.second += 4;
    }

    bypasstrail1 = findpath(start1, des1);
    bypasstrail2 = findpath(start2, des2);

    bool t1blocked = checkifblock(bypasstrail1);

    if (t1blocked)
    {
        return bypasstrail2;
    }
    else
    {
        return bypasstrail1;
    }
}

bool pairisinthegroup(pair<int, int> pforcheck, vector<pair<int, int> >group)
{
    for(int i = 0;i< group.size();i++ )
    {
        if(pforcheck == group[i])
        {
            return true;
        }
    }
    return false;
}

void board::circletransportforemp(pair<int, int> center, pair<int, int> desire) // one step each time.
{
    vector<pair<int, int>> circleindex; // vector for pos around.
    vector<int> validcheckcir;

    validcheckcir.resize(8, 0);

    vector<pair<int, int>> partofcircle;

    pair<int, int> leftup = {center.first - 1, center.second - 1};
    pair<int, int> rightdown = {center.first + 1, center.second + 1};

    partofcircle = findpath(leftup, rightdown);
    for (int i = 0; i < 4; i++)
    {
        circleindex.push_back(partofcircle[i]);
    }

    partofcircle.clear();

    partofcircle = findpath(rightdown, leftup);
    for (int i = 0; i < 4; i++)
    {
        circleindex.push_back(partofcircle[i]);
    }

    // check vailidity
    for (int i = 0; i < 8; i++)
    {
        if (!checkifwithin(circleindex[i]) || checkifblockforpair(circleindex[i]))
        {
            validcheckcir[i] = 1;
        }
    }

    // move the empty to the circle
    vector<pair<int, int>> emptytocir;

    if (emp_squ_pos.second == dealinglinenum && !solvetenspecialcase)
    {
        emptytocir = findpath_yfirst(emp_squ_pos, center);
    }
    else
    {
        emptytocir = findpath(emp_squ_pos, center);
    }



    emptytocir.pop_back(); // delete the last one;

    if(pairisinthegroup(emptytocir[emptytocir.size()-2],circleindex)&&!emptytocir.empty())
    {
        emptytocir.pop_back();
    }

    empgotrail(emptytocir); // move empty to the circle.

    // get empty pos in circle
    int empposcir = 0;

    while (circleindex[empposcir] != emp_squ_pos)
    {
        empposcir++;
    }

    // get desire pos in circle
    int desirepos = 0;
    while (circleindex[desirepos] != desire)
    {
        desirepos++;
    }

//    vector<pair<int, int>> thefinaltrailtomoveempt;

//    vector<pair<int,int>> thefinaltrail2;

//    // when we hit the wall , take another direction.
//    int empposcontainer = empposcir; // container for roll back;
//    bool success = false;
//    while (desirepos != empposcir)
//    {
//        empposcir++;
//        // handle if exceed 7;
//        if (empposcir == 8)
//        {
//            empposcir = 0;
//        }
//        if (validcheckcir[empposcir])
//        {
//            // roll back
//            while (empposcir != empposcontainer)
//            {
//                empposcir--;
//            }
//            thefinaltrailtomoveempt.clear();
//            break;
//        }
//        else
//        {
//            thefinaltrailtomoveempt.push_back(circleindex[empposcir]);
//            if (desirepos == empposcir)
//            {
//                bool success = true;
//            }
//        }
//    }

//    if (!success)
//    {
//        while (desirepos != empposcir)
//        {
//            empposcir--;
//            // handle exceed 0
//            if (empposcir == -1)
//            {
//                empposcir = 7;
//            }
//            thefinaltrailtomoveempt.push_back(circleindex[empposcir]);
//        }
//    }

//    empgotrail(thefinaltrailtomoveempt);

    vector<pair<int,int>> path1;
    bool path1aval = false;
    vector<pair<int,int>> path2;
    bool path2aval = false;

    int empposcontainer = empposcir;

    //deal with clockwise path first
    while(empposcir != desirepos)
    {
        empposcir++;
        if (empposcir == 8)
        {
            empposcir = 0;
        }

        if (validcheckcir[empposcir])
        {
            break;
        }
        else
        {
            path1.push_back(circleindex[empposcir]);
            if(empposcir == desirepos)
            {
                path1aval = true;
            }
        }

    }

    empposcir = empposcontainer;

    while(empposcir != desirepos)
    {
        empposcir--;
        if (empposcir == -1)
        {
            empposcir = 7;
        }

        if(validcheckcir[empposcir])
        {
            break;
        }
        else
        {
            path2.push_back(circleindex[empposcir]);
            if(empposcir == desirepos)
            {
                path2aval = true;
            }
        }
    }

    if(path1aval&&path2aval)
    {
        if(path2.size()>path1.size())
        {
            empgotrail(path1);
        }
        else
        {
            empgotrail(path2);
        }
    }
    else if(path1aval)
    {
        empgotrail(path1);
    }
    else
    {
        empgotrail(path2);
    }


    swaptoemp(center);

    generalshow();
}

void board::circlemethod(int num)
{
    // get dealing line to determine the version of findpath.
    if (num <= 4)
    {
        dealinglinenum = 0;
    }

    if (num > 4 && num <= 8)
    {
        dealinglinenum = 1;
    }

    if (num > 8 && num <= 12)
    {
        dealinglinenum = 2;
    }

    if (num > 12)
    {
        dealinglinenum = 3;
    }
    // get the real pos of the target
    pair<int, int> targetpos = searchbyvalue(num, chart);

    // set unmoveable
    writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);

    vector<pair<int, int>> thetrailreal, thecontainer;

    // special method for 4 and 8 , the tail of the line.

    // check the road
    // first,search for the target number right pos.
    pair<int, int> rightpos = searchbyvalue(num, rightposrecord);

    if (rightpos == targetpos)
    {
        return;
    }

    if (num == 4 || num == 8)
    {
        pair<int, int> special;

        switch (num)
        {
        case 4:
            special.first = 3;
            special.second = 1;
            break;

        case 8:
            special.first = 3;
            special.second = 2;
            break;
        }

        bool specialcase = (targetpos.first == special.first && targetpos.second == special.second);
        bool empisready = (emp_squ_pos.first == 3 && emp_squ_pos.second == 0);
        bool empisready2 = (emp_squ_pos.first == 3 && emp_squ_pos.second == 1);

        if (specialcase && empisready&&num==4||specialcase&&empisready2&&num==8)
        {
            swaptoemp(targetpos);
            return;
        }
        else
        {
            rightpos.first--;
            rightpos.second++;
        }
    }

    thecontainer = findpath(targetpos, rightpos);

    // get the right trail, start included.
    thetrailreal.push_back(targetpos);
    for (int i = 0; i < thecontainer.size(); i++)
    {
        thetrailreal.push_back(thecontainer[i]);
    }

    for (int i = 0; i < thetrailreal.size() - 1; i++)
    {
        pair<int, int> cent = thetrailreal[i];
        pair<int, int> desi = thetrailreal[i + 1];
        circletransportforemp(cent, desi);
    }

    if (num == 4 || num == 8)
    {
        solve4n8_addition_ver1(num);
    }
}

void board::circlediliver(pair<int, int> start, pair<int, int> desti)
{
}

int board::findturingpoint(vector<pair<int, int>> road) // xfirst version
{
    int turning = -1;

    // detect change in second.
    int lastsecond = road[0].second; // initialize with the first point second.
    for (int i = 1; i < road.size(); i++)
    {
        if (road[i].second != lastsecond)
        {
            turning = i;
            break;
        }
        // update the lastsecond
        else
        {
            lastsecond = road[i].second;
        }
    }

    return turning - 1;
    // 0 for always dealing y,turing at the start.
    //-2 for no turing point, always dealing x.
    // others for vaild turing point
}

int board::findturningpoint_yfirst(vector<pair<int, int>> road)
{
    int headse = road[0].second;
    int tailse = road.back().second;

    return headse - tailse;
    // return the index of the turning point , not the pos count.
}

vector<pair<int, int>> board::findpath(pair<int, int> start, pair<int, int> desti)
{
    vector<pair<int, int>> thetrail;

    // deal x

    bool rxisgreater = (start.first < desti.first);
    bool ryisup = (start.second > desti.second);

    if (rxisgreater)
    {
        while (start.first != desti.first)
        {
            start.first++;
            thetrail.push_back(start);
        }
    }
    else
    {
        while (start.first != desti.first)
        {
            start.first--;
            thetrail.push_back(start);
        }
    }

    // deal y,aways above
    // no that is not true when it comes to find path for empty.

    if (ryisup)
    {
        while (start.second != desti.second)
        {
            start.second--;
            thetrail.push_back(start);
        }
    }
    else
    {
        while (start.second != desti.second)
        {
            start.second++;
            thetrail.push_back(start);
        }
    }

    // the trail is not necessarily valid when it comes to finding path for empty square.
    // double check is needed for the result.

    // the trail does not include the very starting point.
    return thetrail;
}

vector<pair<int, int>> board::findpath_yfirst(pair<int, int> start, pair<int, int> desti)
{
    vector<pair<int, int>> thetrail;
    bool rxisgreater = (start.first < desti.first);
    bool ryisup = (start.second > desti.second);

    if (ryisup)
    {
        while (start.second != desti.second)
        {
            start.second--;
            thetrail.push_back(start);
        }
    }
    else
    {
        while (start.second != desti.second)
        {
            start.second++;
            thetrail.push_back(start);
        }
    }

    if (rxisgreater)
    {
        while (start.first != desti.first)
        {
            start.first++;
            thetrail.push_back(start);
        }
    }
    else
    {
        while (start.first != desti.first)
        {
            start.first--;
            thetrail.push_back(start);
        }
    }

    return thetrail;
}

pair<int, int> board::generatenew_start(pair<int, int> base, bool goingx)
{
    vector<pair<int, int>> contx;
    vector<pair<int, int>> conty;

    pair<int, int> x1(base.first, base.second - 1);
    pair<int, int> x2(base.first, base.second + 1);

    pair<int, int> y1(base.first - 1, base.second);
    pair<int, int> y2(base.first + 1, base.second);

    // if one point is invaild , it can only be the case that this point is not within the range.
    bool x1valid = checkifwithin(x1);
    bool y1valid = checkifwithin(y1);

    if (goingx)
    {
        if (x1valid)
        {
            return x1;
        }
        else
        {
            return x2;
        }
    }
    else
    {
        if (y1valid)
        {
            return y1;
        }
        else
        {
            return y2;
        }
    }
}

void board::foursquarespin(pair<int, int> loc, int move)
{
    // loc is upleft

    // emp go clockwise

    // emp should on loc

    pair<int, int> upright(loc.first + 1, loc.second);
    pair<int, int> downright(upright.first, upright.second + 1);
    pair<int, int> downleft(downright.first - 1, downright.second);

    vector<pair<int, int>> sum;
    sum.push_back(loc);
    sum.push_back(upright);
    sum.push_back(downright);
    sum.push_back(downleft);

    vector<pair<int, int>> emppath;

    int reader = 1; // loc not picked at the first time.
    while (move)
    {
        if (reader == 4)
        {
            reader = 0;
        }
        emppath.push_back(sum[reader]);
        reader++;
        move--;
    }

    empgotrail(emppath);
}

void board::trailpourin(vector<pair<int, int>> from, vector<pair<int, int>> &to)
{
    for (int i = 0; i < from.size(); i++)
    {
        to.push_back(from[i]);
    }
}

// to be finished
vector<pair<int, int>> board::findpath_recursion(pair<int, int> start, pair<int, int> desti, vector<pair<int, int>> &editvec)
{
    vector<pair<int, int>> trailcontainer;
    trailcontainer.push_back(start);

    // both are x version.
    trailcontainer = findpath(start, desti);
    int turningpoint = findturingpoint(trailcontainer);

    int blockerindex = -1; // assume that there is no blocker.
    if (checkifblock(trailcontainer))
    {
        // if blocked,find the first blocker.initialize the index to 0 first.
        blockerindex = 0;
        while (!checkifblockforpair(trailcontainer[blockerindex]))
        {
            editvec.push_back(trailcontainer[blockerindex]);
            blockerindex++;
        }

        bool dealingx = (blockerindex <= turningpoint);

        // fake finish
        return editvec;
    }
    else // if we find the not blocked path , we reach the end of the recursion.
    {
        return editvec;
    }
}

// to be finished
vector<pair<int, int>> board::findpath_stronger(pair<int, int> start, pair<int, int> desti)
{
    vector<pair<int, int>> thetrail;
    vector<pair<int, int>> trailcontainer;

    trailcontainer = findpath(start, desti);

    // for blocked case.
    bool goingx = true;
    int lastpoint_y = start.second;

    // check the entire trail ,if blocked get the blocked pos;
    int blockpos = -1;
    for (int i = 0; i < trailcontainer.size(); i++)
    {
        if (trailcontainer[i].second != lastpoint_y)
        {
            goingx = false;
        }
        if (checkifblockforpair(trailcontainer[i]))
        {
            // to be finished.
        }
    }

    // fake finish
    return trailcontainer;
}

vector<pair<int, int>> board::findpathforemp(pair<int, int> dest)
{
    vector<pair<int, int>> therawtrail;

    vector<pair<int, int>> thevalidtrail;

    therawtrail = findpath(emp_squ_pos, dest);

    // check the raw trail

    for (int i = 0; i < therawtrail.size(); i++)
    {
        // dealingx may not function well when the length is short.
        bool dealingx = (therawtrail[i].second == therawtrail[i + 1].second);

        pair<int, int> readytoadd;

        // if blocked
        int valueinchart = getvaluebypair(therawtrail[i], chart);
        pair<int, int> coorpos = searchbyvalue(valueinchart, rightposrecord);
        bool moveable = getvaluebypair(coorpos, moveablecheck);

        if (!moveable)
        {

            if (dealingx)
            {
                // if the blocker is at the corner.
                if (therawtrail[i + 1].second != therawtrail[i].second)
                {
                    thevalidtrail.push_back(bypass_x(therawtrail[i], (therawtrail[i].first > therawtrail[i - 1].first))[0]);
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        thevalidtrail.push_back(bypass_x(therawtrail[i], (therawtrail[i].first > therawtrail[i - 1].first))[i]);
                    }
                }
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    thevalidtrail.push_back(bypass_y(therawtrail[i], (therawtrail[i].second < therawtrail[i - 1].second))[i]);
                }
            }
        }
        else
        {
            thevalidtrail.push_back(therawtrail[i]);
        }
    }
    generalshow();
    return thevalidtrail;
}

void board::solve1to8_ver1(int num)
{
    // get the real pos of the target
    pair<int, int> targetpos = searchbyvalue(num, chart);

    // set unmoveable
    writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);

    vector<pair<int, int>> thetrailreal;

    // special method for 4 and 8 , the tail of the line.

    // check the road
    // first,search for the target number right pos.
    pair<int, int> rightpos = searchbyvalue(num, rightposrecord);

    if (num == 4 || num == 8)
    {
        rightpos.first--;
        rightpos.second++;
    }

    thetrailreal = findpath(targetpos, rightpos);

    for (int i = 0; i < thetrailreal.size(); i++)
    {
        // find the path for empty square
        vector<pair<int, int>> trailforemp;
        trailforemp = findpathforemp(thetrailreal[i]);
        //cout << "Debug: trailforemp.size() = " << trailforemp.size() << endl;
        for (int in = 0; in < trailforemp.size(); in++)
        {
            swaptoemp(trailforemp[in]);
        }
        swaptoemp(targetpos);
        targetpos = searchbyvalue(num, chart);
    }
}

void board::solve4n8_addition_ver1(int num)
{
    pair<int, int> borrowf4(0, 1);
    pair<int, int> borrowf8(0, 2);

    vector<pair<int, int>> pathforemp;
    if (num == 4)
    {

        if (emp_squ_pos.first == 3)
        {
            pair<int, int> newbase(3, 2);
            pathforemp.push_back(newbase); // new base is part of the road.

            vector<pair<int, int>> paircont = findpath(newbase, borrowf4);

            trailpourin(paircont, pathforemp);
        }
        else
        {
            pathforemp = findpath(emp_squ_pos, borrowf4);
        }

        // empgotrail(pathforemp);//move emp to the borrowsquare

        pair<int, int> step2desti(2, 0);
        vector<pair<int, int>> step2 = findpath_yfirst(borrowf4, step2desti); // caution;

        trailpourin(step2, pathforemp);

        pair<int, int> step3pos(2, 1);
        pair<int, int> tail(3, 0);
        pathforemp.push_back(step3pos);

        vector<pair<int, int>> step3 = findpath(step3pos, tail);
        trailpourin(step3, pathforemp);

        vector<pair<int, int>> step4 = findpath(tail, borrowf4);
        trailpourin(step4, pathforemp);

        empgotrail(pathforemp);
    }
    else
    {
        if (emp_squ_pos.first == 3)
        {
            pair<int, int> newbase(3, 3);
            pathforemp.push_back(newbase);

            vector<pair<int, int>> paircont = findpath(newbase, borrowf8);

            trailpourin(paircont, pathforemp);
        }
        else
        {
            pathforemp = findpath(emp_squ_pos, borrowf8);
        }

        pair<int, int> step2desti(2, 1);
        vector<pair<int, int>> step2 = findpath_yfirst(borrowf8, step2desti);

        trailpourin(step2, pathforemp);

        pair<int, int> step3pos(2, 2);
        pair<int, int> tail(3, 1);
        pathforemp.push_back(step3pos);

        vector<pair<int, int>> step3 = findpath(step3pos, tail);
        trailpourin(step3, pathforemp);

        vector<pair<int, int>> step4 = findpath(tail, borrowf8);
        trailpourin(step4, pathforemp);

        empgotrail(pathforemp);
    }
}

vector<pair<int, int>> board::pathgeneratorbypoint(vector<pair<int, int>> pointsum)
{
    vector<pair<int, int>> result;

    int totalsegment = pointsum.size();

    vector<pair<int, int>> cont;
    for (int i = 0; i < pointsum.size() - 1; i++)
    {
        pair<int, int> strt = pointsum[i];
        pair<int, int> dest = pointsum[i + 1];
        cont = findpath_yfirst(strt, dest);
        trailpourin(cont, result);

        cont.clear();
    }

    return result;
}

vector<pair<int, int>> board::pathgeneratorbypoint_xversion(vector<pair<int, int>> pointsum)
{
    vector<pair<int, int>> result;

    int totalsegment = pointsum.size();

    vector<pair<int, int>> cont;
    for (int i = 0; i < pointsum.size() - 1; i++)
    {
        pair<int, int> strt = pointsum[i];
        pair<int, int> dest = pointsum[i + 1];
        cont = findpath(strt, dest);
        trailpourin(cont, result);

        cont.clear();
    }

    return result;
}

void board::solveten()
{
    int num = 10;
    // get ten s pos in chart
    pair<int, int> targetpos = searchbyvalue(num, chart);

    pair<int,int> rightpos = searchbyvalue(num ,rightposrecord);

    if(targetpos == rightpos)
    {
        pair<int, int> pos(0, 3);
        vector<pair<int, int>> pafemp = findpath_yfirst(emp_squ_pos, pos);
        empgotrail(pafemp);

        writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);
        return;
    }




    // first case;
    pair<int, int> empspe(0, 3);
    pair<int, int> tenspe(1, 3);

    // second case;
    pair<int, int> empspe2(1, 2);
    pair<int, int> tenspe2(0, 3);



//    bool empspecial2 = (emp_squ_pos == empspe2);
//    bool tenspecial2 = (targetpos == tenspe2);
//    bool case2 = empspecial2 && tenspecial2;

//    if (case2)
//    {
//        // converting case2 to case1.
//        vector<pair<int, int>> emppa = findpath_yfirst(emp_squ_pos, tenspe2);
//        empgotrail(emppa);
//        case1 = true;
//    }

    if(targetpos==tenspe2)
    {
        pair<int, int> pos(0, 3);
        vector<pair<int, int>> pafemp = findpath_yfirst(emp_squ_pos, pos);
        empgotrail(pafemp);

        targetpos = searchbyvalue(num, chart);
    }

//     writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);

    bool empspecial = (emp_squ_pos == empspe);
    bool tenspecial = (targetpos == tenspe);
    bool case1 = empspecial && tenspecial;

    if (case1)
    {
        // rescure 10 to the 2 line with 9 s offer.
        // set 9 to moveable temporaryly.
        // turn on the special case for x first findpath.
        solvetenspecialcase = true;

        writebypair(searchbyvalue(9, rightposrecord), 1, moveablecheck);

        circlemethod(num);

        // move emp to line 2.
        pair<int, int> step2start(2, 2);

        vector<pair<int, int>> step2trailemp = findpath(emp_squ_pos, step2start);
        empgotrail(step2trailemp);

        writebypair(searchbyvalue(10, rightposrecord), 1, moveablecheck);

        pair<int, int> step3desti(0, 3);
        vector<pair<int, int>> step3 = findpath(emp_squ_pos, step3desti);
        empgotrail(step3);

        circlemethod(10);
        solvetenspecialcase = false;

        writebypair(searchbyvalue(9, rightposrecord), 0, moveablecheck);
        //cout << "10 special case;";
    }
    else
    {
        circlemethod(num);
    }

    pair<int, int> pos(0, 3);
    vector<pair<int, int>> pafemp = findpath_yfirst(emp_squ_pos, pos);
    empgotrail(pafemp);
}

void board::solveeleven()
{
    int num = 11;
    // get eleven s pos in chart
    pair<int, int> targetpos = searchbyvalue(num, chart);
    pair<int,int> rightpos = searchbyvalue(num ,rightposrecord);

    if(targetpos == rightpos)
    {
        pair<int, int> pos(0, 3);
        vector<pair<int, int>> pafemp = findpath_yfirst(emp_squ_pos, pos);
        empgotrail(pafemp);

        writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);
        return;
    }

    writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);

    pair<int, int> special1(1, 3);
    pair<int, int> special2(2, 3);

    pair<int, int> step4(0, 3);

    vector<pair<int, int>> finalpath;

    if (targetpos == special1)
    {
        pair<int, int> step1(1, 2);
        pair<int, int> step2(3, 3);
        pair<int, int> step3(0, 2);
        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos); // emp always be first

        forgene.push_back(step1);
        forgene.push_back(step2);
        forgene.push_back(step3);

        finalpath = pathgeneratorbypoint(forgene);

        finalpath.push_back(step4);
        empgotrail(finalpath);
        //cout << "11-special case 1" << endl;
    }
    else if (targetpos == special2)
    {
        pair<int, int> step1(2, 2);
        pair<int, int> step2(1, 3);
        pair<int, int> step3(0, 2);

        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos); // emp always be first

        forgene.push_back(step1);
        forgene.push_back(step2);
        forgene.push_back(step3);

        finalpath = pathgeneratorbypoint(forgene);

        finalpath.push_back(step4);
        empgotrail(finalpath);
        //cout << "11-special case 2" << endl;
    }
    else
    {
        circlemethod(num);
    }

    pair<int, int> pos(0, 3);
    vector<pair<int, int>> pafemp = findpath_yfirst(emp_squ_pos, pos);
    empgotrail(pafemp);
}

void board::solvetwelve()
{
    pair<int, int> specail1(1, 3);
    pair<int, int> specail2(2, 3);
    pair<int, int> specail3(3, 3);

    int num = 12;
    // get eleven s pos in chart
    pair<int, int> targetpos = searchbyvalue(num, chart);
    pair<int,int> rightpos = searchbyvalue(num ,rightposrecord);

    if(targetpos == rightpos)
    {
        writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);
        return;
    }
    writebypair(searchbyvalue(num, rightposrecord), 0, moveablecheck);

    bool spcase1 = (targetpos == specail1);
    bool spcase2 = (targetpos == specail2);
    bool spcase3 = (targetpos == specail3);

    pair<int, int> step4(0, 3);

    vector<pair<int, int>> emptrail;

    if (spcase1)
    {
        pair<int, int> step1(3, 2);
        pair<int, int> step2(0, 3);
        pair<int, int> step3(2, 2);
        pair<int, int> step4_1(3, 3);
        pair<int, int> step5(0, 2);
        pair<int, int> step6(2, 3);
        pair<int, int> step7 = step5;

        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos);
        forgene.push_back(step1);
        forgene.push_back(step2);
        forgene.push_back(step3);
        forgene.push_back(step4_1);
        forgene.push_back(step5);
        forgene.push_back(step6);
        forgene.push_back(step7);

        emptrail = pathgeneratorbypoint(forgene);
        emptrail.push_back(step4);
        empgotrail(emptrail);
        //cout << "12-special case 1;" << endl;
    }
    else if (spcase2)
    {
        pair<int, int> step1(2, 2);
        pair<int, int> step2(3, 3);
        pair<int, int> step3(0, 2);

        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos); // emp always be first

        forgene.push_back(step1);
        forgene.push_back(step2);
        forgene.push_back(step3);

        emptrail = pathgeneratorbypoint(forgene);
        emptrail.push_back(step4);
        empgotrail(emptrail);
        //cout << "12-special case 2" << endl;
    }
    else if (spcase3)
    {
        pair<int, int> step1(3, 2);
        pair<int, int> step2(2, 3);
        pair<int, int> step3(0, 2);

        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos); // emp always be first

        forgene.push_back(step1);
        forgene.push_back(step2);
        forgene.push_back(step3);

        emptrail = pathgeneratorbypoint(forgene);
        emptrail.push_back(step4);
        empgotrail(emptrail);
        //cout << "12-special case 3" << endl;
    }
    else
    {
        return;
    }
}

void board::solverest()
{

    pair<int, int> inspect(emp_squ_pos.first + 1, emp_squ_pos.second);
    int value = getvaluebypair(inspect, chart);
    if (value == 13)
    {
        pair<int, int> destination(3, 3);
        vector<pair<int, int>> finally = findpath(emp_squ_pos, destination);
        empgotrail(finally);
        //cout << "the problem is solved with luck!" << endl;
        //cout << "total steps for computer: " << totalstep << "." << endl;
    }
    else
    {
        vector<pair<int, int>> step1vec;
        pair<int, int> point1(3, 2);
        pair<int, int> point2(0, 3);

        vector<pair<int, int>> forgene;
        forgene.push_back(emp_squ_pos); // remember to add the empty.
        forgene.push_back(point1);
        forgene.push_back(point2);
        forgene.push_back(point1);

        step1vec = pathgeneratorbypoint(forgene);
        empgotrail(step1vec);

        pair<int, int> swappoint(2, 2);
        swaptoemp(swappoint);

        foursquarespin(swappoint, 4);

        pair<int, int> fifpos = searchbyvalue(15, chart);
        if (fifpos.second == 3)
        {
            foursquarespin(swappoint, 4);
        }

        pair<int, int> destination(3, 3);

        vector<pair<int, int>> xforgene;
        xforgene.push_back(swappoint);
        xforgene.push_back(point2);
        xforgene.push_back(point1);
        xforgene.push_back(point2);
        xforgene.push_back(destination);

        vector<pair<int, int>> step3vec = pathgeneratorbypoint_xversion(xforgene);
        empgotrail(step3vec);



        //cout << "the problem is finally solved!" << endl;
        //cout << "total steps for computer: " << totalstep << "." << endl;
    }
    emit solvedone();
}




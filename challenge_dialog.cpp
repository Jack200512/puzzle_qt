#include "challenge_dialog.h"
#include "player.h"

void challenge_dialog::calculation()
{
    if(challenger_time<bechallenged_time)
    {
        state = 1;
        textforchallenger = "vs."+this->challengedplr->username+" == "+"Win "+QString::number(this->challenger_time)+"-"+QString::number(this->bechallenged_time);
        textforbechallenged = "vs."+this->challenger->username+" == "+"Lose "+QString::number(this->bechallenged_time)+"-"+QString::number(this->challenger_time);
    }
    else if(challenger_time == bechallenged_time)
    {
        state = 3;
        textforchallenger = "vs."+this->challengedplr->username+" == "+"Draw "+QString::number(this->challenger_time)+"-"+QString::number(this->bechallenged_time);
        textforbechallenged = "vs."+this->challenger->username+" == "+"Draw "+QString::number(this->bechallenged_time)+"-"+QString::number(this->challenger_time);
    }
    else
    {
        state = 0;
        textforchallenger = "vs."+this->challengedplr->username+" == "+"Lose "+QString::number(this->challenger_time)+"-"+QString::number(this->bechallenged_time);
        textforbechallenged = "vs."+this->challenger->username+" == "+"Win "+QString::number(this->bechallenged_time)+"-"+QString::number(this->challenger_time);
    }

    this->challengedplr->challenge_msg.push_back(textforbechallenged);
    this->challenger->challenge_msg.push_back(textforchallenger);

    emit battleover();
}

challenge_dialog::challenge_dialog(player * challeger, player * bechalleged, vector<vector<int> > & problemon, QObject *parent):QObject(parent)
{
    challenger = challeger;
    challengedplr = bechalleged;
    theproblem_on = problemon;

}

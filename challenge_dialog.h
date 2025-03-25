#ifndef CHALLENGE_DIALOG_H
#define CHALLENGE_DIALOG_H

#include <QObject>
#include <player.h>

class challenge_dialog : public QObject
{
    Q_OBJECT
    player * challenger;
    player * challengedplr;

public:
    challenge_dialog(player*,player*,QObject * parent = nullptr);

signals:
    void battleover(player *);
};

#endif // CHALLENGE_DIALOG_H

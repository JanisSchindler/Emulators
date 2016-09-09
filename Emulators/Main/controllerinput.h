#ifndef INPUT_H
#define INPUT_H

#include <QObject>
#include <QTimer>
#include <xinput.h>
#include <windows.h>
#include <winerror.h>

#include "logger.h"

namespace Input
{
    class ControllerInput;

    enum Keys
    {
        KeysNone =   0x00,
        Accept =     0x01,
        Back =       0x02,
        Up =         0x04,
        Right =      0x08,
        Down =       0x10,
        Left =       0x20,
        Start =      0x40,
        Exit =       0x80
    };

    // XInput supports up to 4 controllers
    enum Players
    {
      PlayersNone = 0x00,
      P1 =          0x01,
      P2 =          0x02,
      P3 =          0x04,
      P4 =          0x08
    };
}

// create convenient typedef for our imported function
typedef WINAPI DWORD (*GetStateFunc)(DWORD, XINPUT_STATE*);

class ControllerInput : public QObject
{
    Q_OBJECT

public:
    static ControllerInput* getInstance();
    static void cleanup();

signals:
    keyPressed(Input::Keys keys);

private:
    // singleton
    ControllerInput();
    static ControllerInput* sInstance;
    // members
    short mRepeats;
    GetStateFunc mGetState;
    XINPUT_STATE mControllerState;
    Input::Players mFoundPlayers;
    QTimer* mPt_Timer;
    Input::Keys mCurrentKey;
    HINSTANCE mXinputInstance = NULL;
    // methods
    Input::Keys doOnTimer(PXINPUT_STATE state, GetStateFunc func, int playerNo);
    // Flag helpers
    Input::Keys setFlag(Input::Keys current, Input::Keys added);
    Input::Keys resetFlag(Input::Keys current,Input::Keys removed);
    Input::Players setFlag(Input::Players current, Input::Players added);
    Input::Players resetFlag(Input::Players current,Input::Players removed);

private slots:
    void onTimer();
};


#endif // INPUT_H

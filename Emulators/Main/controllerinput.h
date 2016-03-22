#ifndef INPUT_H
#define INPUT_H

#include <xinput.h>
#include <windows.h>
#include <winerror.h>
#include <qobject.h>
#include <qtimer.h>
#include <qdebug.h>

namespace Input
{
    class ControllerInput;

    enum Keys
    {
        None =   0x00,
        Accept = 0x01,
        Back =   0x02,
        Up =     0x04,
        Right =  0x08,
        Down =   0x10,
        Left =   0x20,
        Start =  0x40
    };
}

class ControllerInput : public QObject
{
    Q_OBJECT

public:
    ControllerInput();
    bool isConnected();

signals:
    keyPressed(Input::Keys keys);

private:
    XINPUT_STATE mControllerState;
    int mFoundController;
    QTimer* mPt_Timer;
    Input::Keys mCurrentKey;
    short mRepeats;
    Input::Keys SetFlag(Input::Keys current, Input::Keys added);
    Input::Keys ResetFlag(Input::Keys current,Input::Keys removed);

private slots:
    void onTimer();
};


#endif // INPUT_H

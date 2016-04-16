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
        None =   0x00,
        Accept = 0x01,
        Back =   0x02,
        Up =     0x04,
        Right =  0x08,
        Down =   0x10,
        Left =   0x20,
        Start =  0x40,
        Exit =   0x80
    };
}

class ControllerInput : public QObject
{
    Q_OBJECT

public:
    static ControllerInput* getInstance();
    static void cleanup();

signals:
    keyPressed(Input::Keys keys);

private:
    ControllerInput();
    static ControllerInput* sInstance;

    XINPUT_STATE mControllerState;
    int mFoundController;
    QTimer* mPt_Timer;
    Input::Keys mCurrentKey;
    HINSTANCE mXinputInstance = NULL;

private slots:
    void onTimer();
};


#endif // INPUT_H

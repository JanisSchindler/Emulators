#ifndef INPUT_H
#define INPUT_H

#include <xinput.h>
#include <windows.h>
#include <qobject.h>
#include <qtimer.h>

namespace Input
{
    class ControllerInput;

    enum Keys
    {
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

private slots:
    void onTimer();

};


#endif // INPUT_H

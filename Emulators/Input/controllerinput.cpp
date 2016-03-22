#include "controllerinput.h"

// define stub of the method we want to import
// First get a convenient typedef
typedef WINAPI (*GetStateFunc)(DWORD, XINPUT_STATE*);

// next declare a stub which at least does not crash the program
WINAPI GetStateStub(DWORD, XINPUT_STATE*)
{
  return 0;
}
// and assign it as default
GetStateFunc mGetState = GetStateStub;

ControllerInput::ControllerInput()
{
    // signal no controller found
    mFoundController = -1;
    // try to load the real functions
    HINSTANCE hInstXinput = LoadLibrary((LPCWSTR)"XInput9_1_0.dll");
    DWORD error;

    if(hInstXinput == NULL)
    {
      error = GetLastError();
      return;
    }
    mGetState = (GetStateFunc)GetProcAddress(hInstXinput, (LPCSTR)"XInputGetState");
    if (mGetState == NULL)
    {
      return;
    }

    // reset
    for (int i = 0; i < 4; ++i)
    {
        ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));
    }
    DWORD result;
    // check if any controller is connected and use that
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 4; ++i)
        {
            result = mGetState(j, &mControllerState);
            if (result == ERROR_SUCCESS)
            {
                // found one
                mFoundController = i;
                return;
            }
            // wait for 5 seconds total
            Sleep(250);
        }
    }
    if (mFoundController < 0)
    {
        return;
    }
    // start polling timer
    QTimer* foo = new QTimer(this);
    foo->setInterval(100);
    connect(foo, SIGNAL(timeout()), this, SLOT(onTimer()));
    return;
}

bool ControllerInput::isConnected()
{
    return mFoundController >= 0;
}

void ControllerInput::onTimer()
{
  DWORD result;
  result = mGetState(mFoundController, &mControllerState);
  if (result != ERROR_SUCCESS)
  {
    return;
  }
}


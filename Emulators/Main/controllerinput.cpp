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
  mRepeats = 0;
  // signal no controller found
  mFoundController = -1;
  // try to load the real function
  // TODO maybe clean this up some time in ~ControllerInput or so...
  HINSTANCE hInstXinput = LoadLibrary(L"XInput9_1_0.dll");
  DWORD error;

  if(hInstXinput == NULL)
  {
    error = GetLastError();
    qDebug() << "Error on LoadLibrary" << error;
    return;
  }
  // assign the real function
  mGetState = (GetStateFunc)GetProcAddress(hInstXinput, "XInputGetState");
  if (mGetState == NULL)
  {
    error = GetLastError();
    qDebug() << "Error accessing XInputGetState" << error;
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
        mFoundController = j;
        break;
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
  mPt_Timer = new QTimer(this);
  connect(mPt_Timer, SIGNAL(timeout()), this, SLOT(onTimer()));
  mPt_Timer->start(50);
  return;
}

bool ControllerInput::isConnected()
{
  return mFoundController >= 0;
}

void ControllerInput::onTimer()
{
  ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));
  DWORD result;
  result = mGetState(mFoundController, &mControllerState);
  if (result != ERROR_SUCCESS)
  {
    qDebug() << "Error onTimer" << result;
    return;
  }
  WORD buttons;
  Input::Keys keys = Input::None;
  buttons = mControllerState.Gamepad.wButtons;
  if (buttons & XINPUT_GAMEPAD_A || buttons & XINPUT_GAMEPAD_X)
  {
    keys = SetFlag(keys, Input::Accept);
  }
  if (buttons & XINPUT_GAMEPAD_B || buttons & XINPUT_GAMEPAD_Y)
  {
     keys = SetFlag(keys, Input::Back);
  }
  short thumb = mControllerState.Gamepad.sThumbLX;
  if (buttons & XINPUT_GAMEPAD_DPAD_LEFT || thumb < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = SetFlag(keys, Input::Left);
  }
  if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT || thumb > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = SetFlag(keys, Input::Right);
  }
  thumb = mControllerState.Gamepad.sThumbLY;
  if (buttons & XINPUT_GAMEPAD_DPAD_UP || thumb < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = SetFlag(keys, Input::Up);
  }
  if (buttons & XINPUT_GAMEPAD_DPAD_DOWN || thumb > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = SetFlag(keys, Input::Down);
  }
  if (buttons & XINPUT_GAMEPAD_START)
  {
     keys = SetFlag(keys, Input::Start);
  }
  if (buttons & XINPUT_GAMEPAD_BACK)
  {
     keys = SetFlag(keys, Input::Back);
  }
  // some consistency thingys (reset keys if there are concurrent inputs
  if (keys & Input::Accept && keys & Input::Back)
  {
    keys = ResetFlag(keys, Input::Accept);
    keys = ResetFlag(keys, Input::Back);
  }
  if (keys & Input::Up && keys & Input::Down)
  {
    keys = ResetFlag(keys, Input::Up);
    keys = ResetFlag(keys, Input::Down);
  }
  if (keys & Input::Left && keys & Input::Right)
  {
    keys = ResetFlag(keys, Input::Left);
    keys = ResetFlag(keys, Input::Right);
  }
  if (keys == mCurrentKey)
  {
    if (mRepeats++ < 10)
    {
      // continuous sending of same key every .5 s
      return;
    }
  }
  mRepeats = 0;
  keyPressed(keys);
  mCurrentKey = keys;
  qDebug() << keys;
}

Input::Keys ControllerInput::SetFlag(Input::Keys current, Input::Keys added)
{
  return static_cast<Input::Keys>( static_cast<int> (current) | static_cast<int> (added));
}

Input::Keys ControllerInput::ResetFlag(Input::Keys current,Input::Keys removed)
{
  return static_cast<Input::Keys>( static_cast<int> (current) & ~static_cast<int> (removed));
}
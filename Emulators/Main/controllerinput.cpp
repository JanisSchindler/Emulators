#include "controllerinput.h"

// note that c++ enums are integers in disguise
Input::Keys ControllerInput::setFlag(Input::Keys current, Input::Keys added)
{
  return static_cast<Input::Keys>( static_cast<int> (current) | static_cast<int> (added));
}

Input::Keys ControllerInput::resetFlag(Input::Keys current,Input::Keys removed)
{
  return static_cast<Input::Keys>( static_cast<int> (current) & ~static_cast<int> (removed));
}

Input::Players ControllerInput::setFlag(Input::Players current, Input::Players added)
{
  return static_cast<Input::Players>( static_cast<int> (current) | static_cast<int> (added));
}

Input::Players ControllerInput::resetFlag(Input::Players current,Input::Players removed)
{
  return static_cast<Input::Players>( static_cast<int> (current) & ~static_cast<int> (removed));
}

// initialize static variable for singleton
ControllerInput* ControllerInput::sInstance = NULL;
ControllerInput* ControllerInput::getInstance()
{
  if (NULL == sInstance)
  {
    sInstance = new ControllerInput();
  }
  return sInstance;
}

ControllerInput::ControllerInput()
{
  mRepeats = 0;
  // signal no controller found
  mFoundPlayers = Input::PlayersNone;
  // try to load the real function
  mXinputInstance = LoadLibrary(L"XInput9_1_0.dll");
  DWORD error;

  if(mXinputInstance == NULL)
  {
    error = GetLastError();
    Logger::getInstance()->log("Error on LoadLibary " + error);
    return;
  }
  // assign the function
  mGetState = (GetStateFunc)GetProcAddress(mXinputInstance, "XInputGetState");
  if (mGetState == NULL)
  {
    error = GetLastError();
    Logger::getInstance()->log("Error accessing XInputGetState " + error);
    return;
  }

  // reset
  ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));

  // check if any controller is connected and use that
  for (int i = 0; i < 10; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      DWORD result = mGetState(j, &mControllerState);
      if (result == ERROR_SUCCESS)
      {
        // found one
        switch(j)
        {
          case 0:
           if (mFoundPlayers & Input::P1)
           {
             break;
           }
           mFoundPlayers = setFlag(mFoundPlayers, Input::P1);
           Logger::getInstance()->log("Found controller in slot 1");
           break;
          case 1:
            if (mFoundPlayers & Input::P2)
            {
              break;
            }
            mFoundPlayers = setFlag(mFoundPlayers, Input::P2);
            Logger::getInstance()->log("Found controller in slot 2");
            break;
          case 2:
            if (mFoundPlayers & Input::P3)
            {
              break;
            }
            mFoundPlayers = setFlag(mFoundPlayers, Input::P3);
            Logger::getInstance()->log("Found controller in slot 3");
            break;
          case 3:
            if (mFoundPlayers & Input::P4)
            {
              break;
            }
            mFoundPlayers = setFlag(mFoundPlayers, Input::P4);
            Logger::getInstance()->log("Found controller in slot 4");
            break;
        }
      }
      // wait for .5 seconds total
      Sleep(50);
    }
  }
  if (mFoundPlayers == Input::PlayersNone)
  {
    return;
  }
  // start polling timer
  mPt_Timer = new QTimer(this);
  connect(mPt_Timer, SIGNAL(timeout()), this, SLOT(onTimer()));
  mPt_Timer->start(50);
}

void ControllerInput::cleanup()
{
  if (NULL == sInstance)
  {
    return;
  }
  if (sInstance->mFoundPlayers != Input::PlayersNone)
  {
    sInstance->mPt_Timer->disconnect();
    sInstance->mPt_Timer->stop();
    delete sInstance->mPt_Timer;
  }
  FreeLibrary(sInstance->mXinputInstance);
  sInstance = NULL;
}

Input::Keys ControllerInput::doOnTimer(PXINPUT_STATE state, GetStateFunc func, int playerNo)
{
  ZeroMemory(state, sizeof(XINPUT_STATE));
  DWORD result = func(playerNo, state);
  if (result != ERROR_SUCCESS)
  {
    QString error;
    error.sprintf(("Error in onTimer: %lu"), result);
    Logger::getInstance()->logOnce(error, 0x0001);
    return Input::KeysNone;
  }
  WORD buttons = state->Gamepad.wButtons;
  Input::Keys keys = Input::KeysNone;
  if (buttons & XINPUT_GAMEPAD_A || buttons & XINPUT_GAMEPAD_X)
  {
    keys = setFlag(keys, Input::Accept);
  }
  if (buttons & XINPUT_GAMEPAD_B || buttons & XINPUT_GAMEPAD_Y)
  {
     keys = setFlag(keys, Input::Back);
  }
  short thumb = state->Gamepad.sThumbLX;
  if (buttons & XINPUT_GAMEPAD_DPAD_LEFT || thumb < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = setFlag(keys, Input::Left);
  }
  if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT || thumb > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = setFlag(keys, Input::Right);
  }
  thumb = state->Gamepad.sThumbLY;
  if (buttons & XINPUT_GAMEPAD_DPAD_UP || thumb > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = setFlag(keys, Input::Up);
  }
  if (buttons & XINPUT_GAMEPAD_DPAD_DOWN || thumb < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
  {
     keys = setFlag(keys, Input::Down);
  }
  if (buttons & XINPUT_GAMEPAD_START)
  {
     keys = setFlag(keys, Input::Start);
  }
  if (buttons & XINPUT_GAMEPAD_BACK)
  {
     keys = setFlag(keys, Input::Back);
  }
  byte trigger1 = state->Gamepad.bLeftTrigger;
  byte trigger2 = state->Gamepad.bRightTrigger;
  if (trigger1 > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && trigger2 > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
  {
    keys = setFlag(keys, Input::Exit);
  }
  return keys;
}

void ControllerInput::onTimer()
{
  Input::Keys keys = Input::KeysNone;

  if (mFoundPlayers & Input::P1)
  {
    keys = setFlag(keys, doOnTimer(&mControllerState, mGetState, 0));
  }
  if (mFoundPlayers & Input::P2)
  {
    keys = setFlag(keys, doOnTimer(&mControllerState, mGetState, 1));
  }
  if (mFoundPlayers & Input::P3)
  {
    keys = setFlag(keys, doOnTimer(&mControllerState, mGetState, 2));
  }
  if (mFoundPlayers & Input::P4)
  {
    keys = setFlag(keys, doOnTimer(&mControllerState, mGetState, 3));
  }

  // some consistency thingys (reset keys if there are concurrent inputs)
  if (keys & Input::Accept && keys & Input::Back)
  {
    keys = resetFlag(keys, Input::Accept);
    keys = resetFlag(keys, Input::Back);
  }
  if (keys & Input::Up && keys & Input::Down)
  {
    keys = resetFlag(keys, Input::Up);
    keys = resetFlag(keys, Input::Down);
  }
  if (keys & Input::Left && keys & Input::Right)
  {
    keys = resetFlag(keys, Input::Left);
    keys = resetFlag(keys, Input::Right);
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
  emit(keyPressed(keys));
  mCurrentKey = keys;
}

#include "mocmodel.h"

mocModel::mocModel(ViewModel* model)
{
  Emulator* e1 = new Emulator();
  e1->name = "N64";

  Emulator* e2 = new Emulator();
  e2->name = "Sony Playstation";

  Emulator* e3 = new Emulator();
  e3->name = "Super Nintendo";
  e3->executablePath = L"D:\\Emulators\\SNES\\Snes9x.exe";
  //e3->arguments = L"-fullscreen";

  ROM* r1 = new ROM();
  r1->name = "Super Mario 64";

  ROM* r2 = new ROM();
  r2->name = "Mario Kart 64";

  ROM* r3 = new ROM();
  r3->name = "Goldeneye 64";

  ROM* r4 = new ROM();
  r4->name = "Secret of Mana";
  r4->path = L"D:\\Emulators\\SNES\\ROMS\\Secret of Mana.zip";

  ROM* r5 = new ROM();
  r5->name = "Super Mario World";
  r5->path = L"D:\\Emulators\\SNES\\ROMS\\Super Mario World.zip";

  ROM* r6 = new ROM();
  r6->name = "Chrono Trigger";
  r6->path = L"D:\\Emulators\\SNES\\ROMS\\Chrono Trigger.zip";

  ROM* r7 = new ROM();
  r7->name = "Tekken 3";

  ROM* r8 = new ROM();
  r8->name = "Final Fantasy X";

  ROM* r9 = new ROM();
  r9->name = "";

  model->AddEmulator(e1);
  model->AddEmulator(e2);
  model->AddEmulator(e3);

  model->AddRom(e1, r1);
  model->AddRom(e1, r2);
  model->AddRom(e1, r3);

  model->AddRom(e3, r4);
  model->AddRom(e3, r5);
  model->AddRom(e3, r6);

  model->AddRom(e2, r7);
  model->AddRom(e2, r8);
  model->AddRom(e2, r9);
}

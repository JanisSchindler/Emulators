#include "mocmodel.h"

mocModel::mocModel(ViewModel* model)
{
  Emulator* e1 = new Emulator();
  e1->Name = "N64";

  Emulator* e2 = new Emulator();
  e2->Name = "Sony Playstation";

  Emulator* e3 = new Emulator();
  e3->Name = "Super Nintendo";

  ROM* r1 = new ROM();
  r1->Name = "Super Mario 64";

  ROM* r2 = new ROM();
  r2->Name = "Mario Kart 64";

  ROM* r3 = new ROM();
  r3->Name = "Goldeneye 64";

  ROM* r4 = new ROM();
  r4->Name = "Secret of Mana";

  ROM* r5 = new ROM();
  r5->Name = "Super Mario Land";

  ROM* r6 = new ROM();
  r6->Name = "Final Fantasy III";

  ROM* r7 = new ROM();
  r7->Name = "Tekken 3";

  ROM* r8 = new ROM();
  r8->Name = "Final Fantasy X";

  ROM* r9 = new ROM();
  r9->Name = "";

  model->AddEmulator(e1);
  model->AddEmulator(e2);
  model->AddEmulator(e3);

  model->AddRom(e1, r1);
  model->AddRom(e1, r2);
  model->AddRom(e1, r3);

  model->AddRom(e2, r4);
  model->AddRom(e2, r5);
  model->AddRom(e2, r6);

  model->AddRom(e3, r7);
  model->AddRom(e3, r8);
  model->AddRom(e3, r9);
}

#include "viewmodel.h"

int Emulator::sCount = 0;
int ROM::sCount = 0;

static std::vector<ROM>* sEmptyRoms = new std::vector<ROM>();
static Emulator *sNoEmulator = new Emulator();
static ROM *sNoROM = new ROM();

ViewModel::ViewModel()
{
  sNoEmulator->valid = false;
  sNoROM->valid = false;
}

ViewModel::~ViewModel()
{
  delete sEmptyRoms;
  delete sNoEmulator;
  delete sNoROM;
}

int ViewModel::getEmulatorCount()
{
  return mMapEmulator2Rom.size();
}

std::vector<ROM> ViewModel::getRomsForIndex(int index)
{
  if (index < 0 || (uint)index >= mMapEmulator2Rom.size())
  {
    return *sEmptyRoms;
  }
  const Emulator emulator = getEmulatorForIndex(index);
  if (!emulator.valid)
  {
    return *sEmptyRoms;
  }
  return mMapEmulator2Rom[emulator];
}

Emulator ViewModel::getEmulatorForIndex(int index)
{
  if (index < 0 || (uint)index >= mMapEmulator2Rom.size())
  {
    return *sNoEmulator;
  }
  int current = 0;
  for(std::map<Emulator, std::vector<ROM> >::const_iterator it = mMapEmulator2Rom.begin();
      it != mMapEmulator2Rom.end(); it++)
  {
    if (current == index)
    {
      return it->first;
    }
    ++current;
  }
  return *sNoEmulator;
}

ROM ViewModel::getRomForIndices(int emulatorIndex, int romIndex)
{
  if (romIndex < 0 || emulatorIndex < 0 || (uint)emulatorIndex >= mMapEmulator2Rom.size())
  {
    return *sNoROM;
  }
  Emulator emulator = getEmulatorForIndex(emulatorIndex);
  if (!emulator.valid)
  {
    return *sNoROM;
  }
  std::vector<ROM> roms = mMapEmulator2Rom[emulator];
  if ((uint)romIndex >= roms.size())
  {
    return *sNoROM;
  }
  return roms[romIndex];
}

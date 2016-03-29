#include "viewmodel.h"

int Emulator::sCount = 0;
int ROM::sCount = 0;

static std::vector<const ROM*>* sEmptyRoms = new std::vector<const ROM*>();
static Emulator* sNoEmulator = new Emulator();
static ROM* sNoROM = new ROM();

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

const std::vector<const ROM*>* ViewModel::getRomsForEmulator(const Emulator* emulator)
{
  if (!emulator->valid)
  {
    return sEmptyRoms;
  }
  return mMapEmulator2Rom[emulator];
}

const Emulator* ViewModel::getEmulatorForIndex(int index)
{
  if (index < 0 || (uint)index >= mMapEmulator2Rom.size())
  {
    return sNoEmulator;
  }
  int current = 0;
  for(std::map<const Emulator*, std::vector<const ROM*>* >::const_iterator it = mMapEmulator2Rom.begin();
      it != mMapEmulator2Rom.end(); it++)
  {
    if (current == index)
    {
      return it->first;
    }
    ++current;
  }
  return sNoEmulator;
}

const ROM* ViewModel::getRomForIndex(const Emulator *emulator, int romIndex)
{
  if (romIndex < 0 ||!emulator->valid)
  {
    return sNoROM;
  }
  std::vector<const ROM*>* roms = mMapEmulator2Rom[emulator];
  if ((uint)romIndex >= roms->size())
  {
    return sNoROM;
  }
  return roms->at(romIndex);
}

void ViewModel::AddEmulator(const Emulator* emulator)
{
  mMapEmulator2Rom[emulator] = new std::vector<const ROM*>();
}

void ViewModel::AddRom(const Emulator* emulator, const ROM* rom)
{
  std::vector<const ROM*>* roms = mMapEmulator2Rom[emulator];
  roms->push_back(rom);
}

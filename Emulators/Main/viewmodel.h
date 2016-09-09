#ifndef VIEWMODEL_H
#define VIEWMODEL_H


#include <QAbstractItemModel>
#include <Windows.h>

struct Emulator
{
  static int sCount;
  int index;
  std::string name;
  LPCWSTR executablePath;
  std::wstring arguments;
  bool valid;

  Emulator()
  {
    valid = true;
    index = Emulator::sCount++;
  }

  bool operator !=(const Emulator other) const
  {
    return !(other == *this);
  }

  bool operator == (const Emulator other) const
  {
    return index == other.index;
  }

  bool operator > (const Emulator other) const
  {
    return index > other.index;
  }

  bool operator < (const Emulator other) const
  {
    return index < other.index;
  }

  bool operator >= (const Emulator other) const
  {
    return (other == *this || other > *this);
  }

  bool operator <= (const Emulator other) const
  {
    return (other == *this || other < *this);
  }
};

struct ROM
{
  static int sCount;
  int index;
  std::string name;
  std::wstring path;
  bool valid;

  ROM()
  {
    valid = true;
    index = ROM::sCount++;
  }


  bool operator !=(const ROM other) const
  {
    return !(other == *this);
  }

  bool operator == (const ROM other) const
  {
    return index == other.index;
  }

  bool operator > (const ROM other) const
  {
    return index > other.index;
  }

  bool operator < (const ROM other) const
  {
    return index < other.index;
  }

  bool operator >= (const ROM other) const
  {
    return (other == *this || other > *this);
  }

  bool operator <= (const ROM other) const
  {
    return (other == *this || other < *this);
  }
};

class ViewModel
{
  public:
    ViewModel();
    ~ViewModel();

    int getEmulatorCount();
    const std::vector<const ROM*>* getRomsForEmulator(const Emulator* emulator);
    const Emulator* getEmulatorForIndex(int index);
    const ROM* getRomForIndex(const Emulator* emulator, int romIndex);

    void AddEmulator(const Emulator* emulator);
    void AddRom(const Emulator* emulatorIndex, const ROM* rom);

private:
    std::map<const Emulator*, std::vector<const ROM*>* > mMapEmulator2Rom;
    static std::vector<const ROM*>* sEmptyRoms;
    static Emulator* sNoEmulator;
    static ROM* sNoROM;
};

#endif // VIEWMODEL_H

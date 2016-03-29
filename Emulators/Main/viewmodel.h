#ifndef VIEWMODEL_H
#define VIEWMODEL_H


#include <QAbstractItemModel>

struct Emulator
{
  static int sCount;
  int index;
  std::string Name;
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
  std::string Name;
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
};

#endif // VIEWMODEL_H

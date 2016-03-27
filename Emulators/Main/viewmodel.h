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
    std::vector<ROM> getRomsForIndex(int index);
    Emulator getEmulatorForIndex(int index);
    ROM getRomForIndices(int emulatorIndex, int romIndex);

private:
    std::map<Emulator, std::vector<ROM> > mMapEmulator2Rom;
};

#endif // VIEWMODEL_H

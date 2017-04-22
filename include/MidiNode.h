#ifndef __MIDINODE_H__
#define __MIDINODE_H__

#include <vector>
#include <algorithm>

#define NN_UNDEFINED_MIN -1
#define NN_UNDEFINED_MAX -2
#define NN_MINMAX(note) ((note < 0) ? NN_UNDEFINED_MIN : (note > 127 ? NN_UNDEFINED_MAX : note))

class MidiNode {
public:
  MidiNode() : Channel(0), Amplitude(0), NoteNumber(-1), CurrentNoteOffset(0), Status(0)  {}
  MidiNode(int ch, int nn, int amp, int stat, int offset)
  {
    Channel           = int8(ch);
    Amplitude         = int8(amp);
    NoteNumber        = int8(nn);
    CurrentNoteOffset = int8(offset);
    Status            = int8(stat);
  }

  MidiNode(IMidiMsg *nMsg, int mod = 0)
  {
    Channel           = int8(nMsg->Channel());
    Amplitude         = int8(nMsg->Velocity());
    NoteNumber        = int8(nMsg->NoteNumber());
    CurrentNoteOffset = int8(mod);
    Status            = int8(nMsg->StatusMsg());
  }

  int GetNoteMod_Unchecked() { return int(NoteNumber) + int(CurrentNoteOffset); }
  int GetNoteMod() { return NN_MINMAX(GetNoteMod_Unchecked()); }
  bool CanDo(int kOffset) { const int nn = GetNote() + kOffset; return (nn > 0) && (nn < 128); }

  int GetAmp() { return int(Amplitude); }
  int GetNote() { return int(NoteNumber); }
  int GetChannel() { return int(Channel); }

private:
  int8 NoteNumber, Amplitude, CurrentNoteOffset, Status, Channel;
};

typedef std::vector<MidiNode> NoteVector;

class SortedNotes : public NoteVector
{
  static bool sort_notes(MidiNode a, MidiNode b) { return (a.GetNote() < b.GetNote()); }
public:
  SortedNotes::SortedNotes() : NoteVector(0){}
  SortedNotes::SortedNotes(int count) : NoteVector(count){}
  SortedNotes::~SortedNotes() {}

  /**
  get the index of a particular value, or -1 on fail.

  @param value    the value to be found; (GetNoteMod())
  @param start    start index [default=0] */
  inline int SortedNotes::get_index(int value, int start = 0) {
    if (start > size()) return -1;
    for (int i = start; i < size(); i++)
      if (at(i).GetNote() == value) return i;
    return -1;
  }
  /**
  sets found index of the (GetNoteMod()) value to `found_index` or -1 on fail
  and returns `true` if the `value` is present in the vector.
  @param value         the droid we are looking for
  @param found_index   [out] index or -1 is placed here. */
  inline bool SortedNotes::has_value(int value, int found_index = -1)
  {
    int index = get_index(value);
    found_index = int(index);
    return index >= 0;
  }

  /**
  adds into the stack in sequential order
  @param value    the value to be added. */
  bool SortedNotes::add(MidiNode value)
  {
    int known_index = -1;
    if (has_value(value.GetNote(), known_index)) return false;

    push_back(value);
    std::sort(begin(), end(), &sort_notes);
    return true;
  }
  

  /**
  remove a particular value from the vector
  @param value     the number to be removed */
  bool SortedNotes::remove(int value)
  {
    if (!has_value(value)) return false;
    //const SortedNotes myVec = GetConst();
    for (auto it = begin(); it != end(); it++)
    {
      if (((*it).GetNote() == value) && (it != end()))
      {
        this->erase(it);
        this->shrink_to_fit();
        return true;
      }
    }
    return false;
    //auto it = std::find(begin(), end(), value);
    //if (it != end()) erase(it); // erase(std::remove(begin(), end(), value), end()); // didn't work
    //this->shrink_to_fit();
  }
};


#endif
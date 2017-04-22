#ifndef __SORTED_INTS_H__
#define __SORTED_INTS_H__
/*
just some modifications to standard vector applying a sort algo.
  - get_index of a particular int
  - has_value tells you if it has a value calling on get_index
  - add
  - remove
*/

#if _WIN32
#pragma once
#endif

#include <vector>
#include <algorithm>

#ifndef _hasint32_
#define _hasint32_
// not so important. am just used to looking at it.
typedef unsigned __int32 uint32;
typedef signed __int32   int32;
#endif

//typedef std::vector<signed __int32> int32_v;
typedef std::vector<int32> int32_v;


class SortedInt : public int32_v
{
static bool sort_me(int a, int b) { return (a < b); }
public:
  SortedInt() : int32_v(0){}
  SortedInt(int count) : int32_v(count){}
  ~SortedInt() {}

  /**
  get the index of a particular value, or -1 on fail.

  @param value    the value to be found
  @param start    start index [default=0] */
  inline int get_index(int value, int start = 0) const {
    if (start > size()) return -1;
    for (int i = start; i < size(); i++) if (at(i) == value) return i;
    return -1;
  }
  /**
  sets found index of the value to `found_index` or -1 on fail
  and returns `true` if the `value` is present in the vector.
  @param value         the droid we are looking for
  @param found_index   [out] index or -1 is placed here. */
  inline bool has_value(int value, int found_index = -1)
  {
    found_index = get_index(value);
    return found_index >= 0;
  }

  /**
  adds into the stack in sequential order
  @param value    the value to be added. */
  bool add(int value)
  {
    int known_index = -1;
    if (has_value(value, known_index)) return false;

    push_back(value);
    std::sort(begin(), end(), sort_me);
    return true;
  }

  /**
  remove a particular value from the vector
  @param value     the number to be removed */
  void remove(int value) {
    if (!has_value(value)) return;
    auto it = std::find(begin(), end(), value);
    if (it != end()) erase(it); // erase(std::remove(begin(), end(), value), end()); // didn't work
    this->shrink_to_fit();
  }
};




#endif
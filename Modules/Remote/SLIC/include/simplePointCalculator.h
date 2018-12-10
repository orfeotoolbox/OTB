#ifndef simplePointCalculator_h
#define simplePointCalculator_h

#include <array>
#include <vector>
#include <iostream>
#include <bitset>
template <class IteratorType, class LabelType>
class simplePointCalculator{
public:
  int isSimpleLabel (IteratorType const& neighborhoodLabelsBegin, IteratorType const& neighborhoodLabelsEnd, LabelType const& evalLabel) const;
  void generateInitializerList();
private:
  constexpr static std::array<bool,256> T4Table{0,0,1,1,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,1,1,1,1,1};
};
#include "simplePointCalculator.txx"
#endif


#ifndef simplePointCalculator_txx
#define simplePointCalculator_txx

#include "simplePointCalculator.h"

template <class IteratorType, class LabelType>
constexpr
std::array<bool,256>
simplePointCalculator<IteratorType,LabelType>::T4Table;

template <class IteratorType, class LabelType>
int
simplePointCalculator<IteratorType,LabelType>
::isSimpleLabel (IteratorType const& neighborhoodLabelsBegin, IteratorType const& neighborhoodLabelsEnd, LabelType const& evalLabel) const {
  // read in table if evalLabel is simple wrt the neighborhoodLabels
  // simpleKey is 8 bits representing the neighborhood in order
  // b7 b6 b5
  // b4 XX b3
  // b2 b1 b0

  unsigned char simpleKey = 0;
  //read neighborhood backwards
  IteratorType n = neighborhoodLabelsEnd-1;
  for (; n > neighborhoodLabelsBegin-1; --n){
    //Discard central label
    if(n != neighborhoodLabelsBegin+4)
      {
	if (*n == evalLabel)
	  {
	    simpleKey++;
	  }
	if(n != neighborhoodLabelsBegin)
	  {
	    simpleKey <<= 1;
	  }
      }
  }
  return simplePointCalculator<IteratorType,LabelType>::T4Table[simpleKey];
}

template <class IteratorType, class LabelType>
void
simplePointCalculator<IteratorType,LabelType>
::generateInitializerList(){
  std::array<bool,8> ib;
  std::array<int,256> ar;
  for(int i = 0 ; i < 255 ; ++i)
    {
      //Convert number to bit table
      for (int bit = 0; bit < 8 ; ++bit)
	{
	  ib[bit] = (i >> bit) & 1;
	}
      int n=0;
      for (int bit = 1; bit < 8 ; ++bit)
	{
	  n += (ib[bit] == 0 && ib[bit-1] == 1);
	}
      if(ib[7] == 1 && ib[0] == 0)
	{
	  n++;
	}
      ar[i] = (n == 1)&&(ib[1] || ib[3] || ib[5] || ib[7]);
    }
  ar[255]=1;
  bool correct = true;
  std::cout<<"{";
  for (unsigned int i = 0 ; i < 256; i++)
    {
      std::bitset<8> newbits(i);
      std::bitset<8> oldbits;
      oldbits.set(0,newbits[0]);
      oldbits.set(1,newbits[1]);
      oldbits.set(2,newbits[2]);
      oldbits.set(3,newbits[4]);
      oldbits.set(4,newbits[7]);
      oldbits.set(5,newbits[6]);
      oldbits.set(6,newbits[5]);
      oldbits.set(7,newbits[3]);
      std::cout << ar[oldbits.to_ulong()] << (i < 255 ? "," : "");
      correct &= (ar[oldbits.to_ulong()] == simplePointCalculator<IteratorType,LabelType>::T4Table[i]);
    }
  std::cout << "}" << std::endl;
  
  std::cout << (correct ? "success" : "fail") << std::endl;
}
#endif


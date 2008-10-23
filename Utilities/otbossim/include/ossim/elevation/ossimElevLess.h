//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
// Function object (functor) to for stl's sort method for sorting
// a vector of ossimElevSource* by best post spacing.
//*****************************************************************************
//  $Id: ossimElevLess.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimElevLess_HEADER
#define ossimElevLess_HEADER

#include <functional>

#include <ossim/elevation/ossimElevSource.h>

class ossimElevLess :
           public std::binary_function<const ossimRefPtr<ossimElevSource>,
                  const ossimRefPtr<ossimElevSource>, bool>
{
public:
   bool operator() (const ossimRefPtr<ossimElevSource> a, const ossimRefPtr< ossimElevSource> b) const
   {
      return (a->getMeanSpacingMeters() < b->getMeanSpacingMeters());
   }
};

#endif

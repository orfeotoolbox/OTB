//*****************************************************************************
// FILE: ossimEcefVector.cpp
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of a 3D vector object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
//   NOTE: There is no associated ossimEcefVector.cc file. All methods are
//         inlined here
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefVector.cpp 9531 2006-09-11 11:18:27Z dburken $

#include <sstream>
#include <ossim/base/ossimEcefVector.h>

ossimString ossimEcefVector::toString(ossim_uint32 precision) const
{
   return theData.toString(precision);
}

void ossimEcefVector::toPoint(const std::string& s)
{
   theData.toPoint(s);
}

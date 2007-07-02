//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains implementation of class ossimGeoid
//
// SOFTWARE HISTORY:
//>
//   17Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#include <ossim/base/ossimGeoid.h>

RTTI_DEF2(ossimGeoid, "ossimGeoid", ossimObject, ossimErrorStatusInterface)

ossimGeoid::ossimGeoid()
   :
      ossimObject(),
      ossimErrorStatusInterface()
{}

ossimGeoid::~ossimGeoid()
{}

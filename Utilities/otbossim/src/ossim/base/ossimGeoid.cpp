//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains implementation of class ossimGeoid
//
// SOFTWARE HISTORY:
//>
//   17Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************

#include <ossim/base/ossimGeoid.h>

RTTI_DEF2(ossimGeoid, "ossimGeoid", ossimObject, ossimErrorStatusInterface)
RTTI_DEF1(ossimIdentityGeoid, "ossimIdentityGeoid", ossimGeoid)

ossimGeoid::ossimGeoid()
   :
      ossimObject(),
      ossimErrorStatusInterface()
{}

ossimGeoid::~ossimGeoid()
{}

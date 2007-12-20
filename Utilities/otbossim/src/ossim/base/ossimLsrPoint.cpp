//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Log$
//  Revision 1.4  2006/11/28 21:09:36  gpotts
//  Fixing copyrights
//
//  Revision 1.3  2006/06/13 19:10:52  dburken
//  Global include path change.
//
//  Revision 1.2  2003/05/02 15:34:17  dburken
//  Fixed build errors on AIX VisualAge compiler.  <dburken@imagelinks.com>
//
//  Revision 1.1  2001/08/13 21:29:18  okramer
//  Initial delivery of ECEF and LSR suite. (okramer@imagelinks.com)
//

#include <ossim/base/ossimLsrPoint.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimLsrVector.h>
#include <ossim/base/ossimMatrix3x3.h>

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(ossimLsrPoint, space)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimLsrPoint& convert_this,
                             const ossimLsrSpace& new_space)
   : theLsrSpace(new_space)
{
   initialize(ossimEcefPoint(convert_this));
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(const ossimLsrPoint&  convert_this)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimGpt&  gpt,
                             const ossimLsrSpace& space)
   : theLsrSpace(space)
{
   initialize(ossimEcefPoint(gpt));
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrPoint(const ossimLsrPoint&  convert_this)
//  
//*****************************************************************************
ossimLsrPoint::ossimLsrPoint(const ossimEcefPoint& convert_this,
                             const ossimLsrSpace& space)
   : theLsrSpace(space)
{
   initialize(convert_this);
}

//*****************************************************************************
//  PROTECTED METHOD: ossimLsrPoint::initialize(ossimEcefPoint)
//  
//  Convenience method used by several constructors for initializing theData
//  given an ECEF point. Assumes theLsrSpace has been previously initialized.
//  
//*****************************************************************************
void ossimLsrPoint::initialize(const ossimEcefPoint& ecef_point)
{
   if(ecef_point.hasNans())
   {
      makeNan();
   }
   else
   {
      //
      // Translate to new space given the space's offset origin:
      //
      ossimColumnVector3d xlated ((ecef_point - theLsrSpace.origin()).data());
      
      //
      // Rotate by the inverse (transpose) of the LSR-to-ECEF rot matrix:
      //
      theData = theLsrSpace.ecefToLsrRotMatrix() * xlated;
   }
}

//*****************************************************************************
//   OPERATOR: ossimLsrPoint::operator-(ossimLsrPoint)
//  
//*****************************************************************************
ossimLsrVector ossimLsrPoint::operator-(const ossimLsrPoint& p) const
{
   if(hasNans()||p.hasNans()||(theLsrSpace != p.lsrSpace()))
   {
      theLsrSpace.lsrSpaceErrorMessage();
      return ossimLsrVector(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);
      
   }
   return ossimLsrVector(theData-p.theData, theLsrSpace);
}

//*****************************************************************************
//  OPERATOR: ossimLsrPoint::operator+(ossimLsrVector)
//  
//*****************************************************************************
ossimLsrPoint ossimLsrPoint::operator+(const ossimLsrVector& v) const
{
   if(hasNans()||v.hasNans()||(theLsrSpace != v.lsrSpace()))
   {
      ossimLsrSpace::lsrSpaceErrorMessage();
      return ossimLsrPoint(ossim::nan(), ossim::nan(), ossim::nan(), theLsrSpace);
      
   }
   return ossimLsrPoint(theData + v.data(), theLsrSpace);
}


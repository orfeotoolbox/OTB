//*****************************************************************************
// FILE: ossimEcefPoint.cpp
//
// License:  See top level LICENSE.txt file.
//
// DESCRIPTION:
//   Contains declaration of a 3D point object in the Earth-centered, earth
//   fixed (ECEF) coordinate system.
//
// SOFTWARE HISTORY:
//>
//   08Aug2001  Oscar Kramer (http://www.oscarkramer.com)
//              Initial coding.
//<
//*****************************************************************************
//  $Id: ossimEcefPoint.cpp 11399 2007-07-26 13:39:54Z dburken $

#include <ostream>

#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGeocent.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimEcefVector.h>

//*****************************************************************************
//  CONSTRUCTOR: ossimEcefPoint(const ossimGpt&  convert_this)
//  
//*****************************************************************************
ossimEcefPoint::ossimEcefPoint(const ossimGpt&  gpt)
{
   if(!gpt.isHgtNan())
   {
      gpt.datum()->ellipsoid()->latLonHeightToXYZ(gpt.latd(),
                                                  gpt.lond(),
                                                  gpt.height(),
                                                  theData[0],
                                                  theData[1],
                                                  theData[2]);
   }
   else
   {
      gpt.datum()->ellipsoid()->latLonHeightToXYZ(gpt.latd(),
                                                  gpt.lond(),
                                                  0.0,
                                                  theData[0],
                                                  theData[1],
                                                  theData[2]);
   }
}

ossimEcefPoint::ossimEcefPoint(const ossimDpt3d& pt)
   :theData(pt.x, pt.y, pt.z)
{
}

//*****************************************************************************
//  METHOD: ossimEcefPoint::operator-(ossimEcefPoint)
//*****************************************************************************
ossimEcefVector ossimEcefPoint::operator-(const ossimEcefPoint& p) const
{
   return ossimEcefVector(theData - p.theData);
}

//*****************************************************************************
//  METHOD: ossimEcefPoint::operator+(ossimEcefVector)
//*****************************************************************************
ossimEcefPoint ossimEcefPoint::operator+(const ossimEcefVector& v) const
{
   return ossimEcefPoint(theData + v.data());
}

//*****************************************************************************
//  METHOD: ossimEcefPoint::operator-(ossimEcefVector)
//*****************************************************************************
ossimEcefPoint ossimEcefPoint::operator-(const ossimEcefVector& v) const
{
   return ossimEcefPoint(theData - v.data());
}

ossimString ossimEcefPoint::toString(ossim_uint32 precision) const
{
   return theData.toString(precision);
}

void ossimEcefPoint::toPoint(const std::string& s)
{
   theData.toPoint(s);
}

void ossimEcefPoint::print(std::ostream& os) const
{
   if(isNan())
   {
      os << "(ossimEcefPoint) " << "nan nan nan";
   }
   
   os << "(ossimEcefPoint) " << theData;
}

std::ostream& operator<<(std::ostream& os , const ossimEcefPoint& instance)
{
   instance.print(os); return os;
}

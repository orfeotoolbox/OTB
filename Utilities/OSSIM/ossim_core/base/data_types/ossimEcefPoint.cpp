//*****************************************************************************
// FILE: ossimEcefPoint.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
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
//  $Id: ossimEcefPoint.cpp,v 1.9 2005/07/29 20:47:04 dburken Exp $

#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimGeocent.h>
#include <base/data_types/ossimDpt3d.h>
#include <base/data_types/datum/ossimDatum.h>
#include <base/data_types/ellipse/ossimEllipsoid.h>
#include <base/data_types/ossimEcefVector.h>

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

std::ostream& operator<<(std::ostream& os , const ossimEcefPoint& instance)
{
   instance.print(os); return os;
}

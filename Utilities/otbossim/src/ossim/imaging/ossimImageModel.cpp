//-----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class definition of ossimImageModel.
//
//-----------------------------------------------------------------------------
// $Id$

#include <string>

#include <ossim/imaging/ossimImageModel.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimException.h>
#include <ossim/imaging/ossimImageHandler.h>

RTTI_DEF1(ossimImageModel, "ossimImageModel", ossimObject);

ossimImageModel::ossimImageModel()
   : ossimObject(),
     theSubImageOffset(),
     theDecimationFactors(),
     theLines(0),
     theSamples(0),
     theTargetRrds(0)
{
}

ossimImageModel::~ossimImageModel()
{
}

void ossimImageModel::initialize(const ossimImageHandler& ih)
{
   theSubImageOffset = ossimDpt();
   theLines          = ih.getNumberOfLines(0);
   theSamples        = ih.getNumberOfSamples(0);
   ih.getDecimationFactors(theDecimationFactors);
}

void ossimImageModel::rnToR0(ossim_uint32 rrds,
                             const ossimDpt& rnPt,
                             ossimDpt& r0Pt) const
{
   if ( rrds < theDecimationFactors.size() )
   {
      r0Pt.x = rnPt.x / theDecimationFactors[rrds].x;
      r0Pt.y = rnPt.y / theDecimationFactors[rrds].y;
   }
   else
   {
      std::string e = "ossimImageModel::rnToR0 rrds out of range!";
      throw ossimException(e);
   }
}
void ossimImageModel::rnToR0(const ossimDpt& rnPt, ossimDpt& r0Pt) const
{
   rnToR0(theTargetRrds, rnPt, r0Pt);
}
   
void ossimImageModel::r0ToRn(ossim_uint32 rrds,
                             const ossimDpt& r0Pt,
                             ossimDpt& rnPt) const
{
   if ( rrds < theDecimationFactors.size() )
   {
      rnPt.x = r0Pt.x * theDecimationFactors[rrds].x;
      rnPt.y = r0Pt.y * theDecimationFactors[rrds].y;
   }
   else
   {
      std::string e = "ossimImageModel::r0ToRn rrds out of range!";
      throw ossimException(e);
   }
}

void ossimImageModel::r0ToRn(const ossimDpt& r0Pt, ossimDpt& rnPt) const
{
   r0ToRn(theTargetRrds, r0Pt, rnPt);
}

void ossimImageModel::getSubImageOffset(ossim_uint32 rrds,
                                        ossimDpt& offset) const
{
   if ( rrds < theDecimationFactors.size() )
   {
      offset.x = theSubImageOffset.x * theDecimationFactors[rrds].x;
      offset.y = theSubImageOffset.y * theDecimationFactors[rrds].y;
   }
   else
   {
      std::string e = "ossimImageModel::getSubImageOffset rrds out of range!";
      throw ossimException(e);
   } 
}

void ossimImageModel::getImageRectangle(ossim_uint32 rrds,
                                        ossimDrect& rect) const
{
   if ( rrds < theDecimationFactors.size() )
   {
      ossim_float64 lrX = theSamples * theDecimationFactors[rrds].x - 1.0;
      ossim_float64 lrY = theLines   * theDecimationFactors[rrds].y - 1.0;
      ossimDrect r(0.0, 0.0, lrX, lrY);
      rect = r;
   }
   else
   {
      std::string e = "ossimImageModel::getImageRectangle rrds out of range!";
      throw ossimException(e);
   } 
}

void ossimImageModel::getBoundingRectangle(ossim_uint32 rrds,
                                           ossimDrect& rect) const
{
   if ( rrds < theDecimationFactors.size() )
   {
      ossim_float64 urX = theSubImageOffset.x * theDecimationFactors[rrds].x;
      ossim_float64 urY = theSubImageOffset.y * theDecimationFactors[rrds].y;
      
      ossim_float64 lrX = urX + theSamples*theDecimationFactors[rrds].x - 1.0;
      ossim_float64 lrY = urY + theLines  *theDecimationFactors[rrds].y - 1.0;
      ossimDrect r(0, 0, lrX, lrY);
      rect = r;
   }
   else
   {
      std::string e =
         "ossimImageModel::getBoundingRectangle rrds out of range!";
      throw ossimException(e);
   }  
}
ossim_uint32 ossimImageModel::getNumberOfDecimationLevels()const
{
   return (ossim_uint32)theDecimationFactors.size();
}

void ossimImageModel::setTargetRrds(ossim_uint32 rrds)
{
   theTargetRrds = rrds;
}

ossim_uint32 ossimImageModel::getTargetRrds() const
{
   return theTargetRrds;
}

   


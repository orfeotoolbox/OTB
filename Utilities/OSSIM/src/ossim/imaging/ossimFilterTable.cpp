//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//----------------------------------------------------------------------------
// $Id: ossimFilterTable.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <iostream>
#include <ossim/imaging/ossimFilterTable.h>
#include <ossim/imaging/ossimFilter.h>

ossimFilterTable::ossimFilterTable()
   :theWeights(0),
    theWidth(0),
    theHeight(0),
    theWidthHeight(0),
    theXSupport(0),
    theYSupport(0),
    theFilterSteps(0)
{
   
}

ossimFilterTable::~ossimFilterTable()
{
   if(theWeights)
   {
      delete [] theWeights;
      theWeights = 0;
   }
}

void ossimFilterTable::buildTable(ossim_uint32  filterSteps,
                                  const ossimFilter& filter)
{
   buildTable(filterSteps, filter, filter);
}

void ossimFilterTable::buildTable(ossim_uint32  filterSteps,
                                  const ossimFilter& xFilter,
                                  const ossimFilter& yFilter)
{
   ossim_int32 subpixelSample=0;
   ossim_int32 subpixelLine=0;
   double dx   = 0.0;
   double dy   = 0.0;
   double kernelH   = 0.0;
   double kernelV   = 0.0;
   double x = 0;
   double y = 0;

   double xsupport = ceil(xFilter.getSupport());
   double ysupport = ceil(yFilter.getSupport());
   double left    = 0;
   double right   = 0;
   double top     = 0;
   double bottom  = 0;

   theXSupport = (ossim_uint32)xsupport;
   theYSupport = (ossim_uint32)ysupport;
   theFilterSteps = filterSteps;
   theWidth  = (2*theXSupport);
   theHeight = (2*theYSupport);
   theWidthHeight = theWidth*theHeight;
   
   allocateWeights();
   left   = -(xsupport-1);
   right  = xsupport;
   top    = -(ysupport-1);
   bottom = ysupport;
   ossim_uint32 idx = 0;
      
   for (subpixelLine = 0; subpixelLine < (int)filterSteps; 
        subpixelLine++)
   {
     
     // Calculate subpixel sample step.
     // ---------------------------------- 
     dy = subpixelLine / (double)(filterSteps);
     for (subpixelSample = 0; subpixelSample < (int)filterSteps; 
	  subpixelSample++)
       {
      
	 // Calculate subpixel sample step.
	 // ---------------------------------- 
	 dx = subpixelSample / (double)(filterSteps);
	 
	 for (kernelV=top; kernelV<=bottom;
	      kernelV++)
	   {
	     y = kernelV - dy;
	     double tempWeight = yFilter.filter(y, yFilter.getSupport());
	     for(kernelH=left; kernelH<=right;++kernelH)
	       {
		 x = kernelH - dx;
         
		 // Get the weight for the current pixel.
		 //   ----------------------------------------
		 theWeights[idx] = tempWeight*xFilter.filter(x, xFilter.getSupport());
		 ++idx;
	       }
	   }
       }
   }
}

ossim_uint32 ossimFilterTable::getWidthByHeight()const
{
   return theWidthHeight;
}

ossim_uint32 ossimFilterTable::getXSupport()const
{
   return theXSupport;
}

ossim_uint32 ossimFilterTable::getYSupport()const
{
   return theYSupport;
}

ossim_uint32 ossimFilterTable::getWidth()const
{
   return theWidth;
}

ossim_uint32 ossimFilterTable::getHeight()const
{
   return theHeight;
}

void ossimFilterTable::allocateWeights()
{
   if(theWeights)
   {
      delete [] theWeights;
      theWeights = 0;
   }

   ossim_uint32 size = (theWidthHeight*(theFilterSteps*theFilterSteps));

   if(size)
   {
      theWeights = new double[size];
   }
}

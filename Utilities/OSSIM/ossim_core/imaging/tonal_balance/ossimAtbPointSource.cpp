//*****************************************************************************
// FILE: ossimAtbPointSource.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains implementation of class ossimAtbPointSource.
//   This object provides the statistics associated with a given point on a
//   given image corresponding to a matchpoint. A matchpoint contains a
//   collection of these point sources, one for each contributing image.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbPointSource.cpp,v 1.5 2005/02/11 15:07:34 dburken Exp $

#include "imaging/ossimImageSource.h"
#include "imaging/ossimImageData.h"
#include "imaging/factory/ossimImageDataFactory.h"
#include "ossimGridRemapSource.h"
#include "ossimGridRemapEngine.h"
#include "ossimAtbPointSource.h"

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimImageSource:exec");
static ossimTrace traceDebug ("ossimImageSource:debug");

static const int DEFAULT_KERNEL_SIZE = 18;  // recommend odd number

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimAtbPointSource
//  
//*****************************************************************************
ossimAtbPointSource::ossimAtbPointSource()
   : theRemapSource (0),
     theGridRemapEngine (0),
     theKernelSize (DEFAULT_KERNEL_SIZE),
     theViewPointIsValid (false)
{
   static const char MODULE[] = "ossimAtbPointSource Default Constructor";
   if (traceExec())  CLOG << "entering..." << endl;

   if (traceExec())  CLOG << "returning..." << endl;
}
      

//*****************************************************************************
//  CONSTRUCTOR: ossimAtbPointSource(image_source)
//  
//*****************************************************************************
ossimAtbPointSource::ossimAtbPointSource(ossimGridRemapSource* source,
                                         const ossimDpt&    view_point)
   : theRemapSource (source),
     theGridRemapEngine (source->getRemapEngine()),
     theKernelSize (DEFAULT_KERNEL_SIZE),
     theViewPointIsValid (false)
{
   static const char MODULE[] = "ossimAtbPointSource Default Constructor";
   if (traceExec())  CLOG << "entering..." << endl;

   setViewPoint(view_point);
   
   if (traceExec())  CLOG << "returning..." << endl;
}


//*****************************************************************************
//  DESTRUCTOR: ~ossimAtbPointSource()
//  
//*****************************************************************************
ossimAtbPointSource::~ossimAtbPointSource()
{
}

//*****************************************************************************
//  METHOD: ossimAtbPointSource::setRemapSource()
//  
//  Sets the pointer to the source of pixels used to compute the stats.
//  
//*****************************************************************************
void ossimAtbPointSource::setRemapSource(ossimGridRemapSource* source)
{
   static const char MODULE[] = "ossimAtbPointSource::setRemapSource()";
   if (traceExec())  CLOG << "entering..." << endl;

   theRemapSource = source;

   //***
   // Also fetch the remap engine which should be a derived ossimAtbRemapEngine:
   //***
   theGridRemapEngine = theRemapSource->getRemapEngine();
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbPointSource::setViewPoint()
//  
//  Sets the view coordinates corresponding to this point.
//  
//*****************************************************************************
void ossimAtbPointSource::setViewPoint(const ossimDpt& view_point)
{
   static const char MODULE[] = "ossimAtbPointSource::setViewPoint()";
   if (traceExec())  CLOG << "entering..." << endl;

   theViewPoint = view_point;
   
   if ((theRemapSource) &&
       (theRemapSource->getBoundingRect().pointWithin(theViewPoint)))
      theViewPointIsValid = true;
   else
      theViewPointIsValid = false;
         
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbPointSource::getSourceValue()
//  
//  Returns the computed value vector corresponding to the region about the
//  view point. The definition of "value" is implemented by theGridRemapEngine.
//
//*****************************************************************************
void ossimAtbPointSource::getSourceValue(void* value_vector)
{
   static const char MODULE[] = "ossimAtbPointSource::getSourceValue()";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Verify that members are initialized before processing:
   //***
   if ((!theViewPointIsValid) || (!theRemapSource) || (!theGridRemapEngine))
   {
      CLOG <<"ERROR: This object was not properly initialized before attempting"
           <<" to compute target value! Returning usassigned vector..."<<endl;
      return;
   }

   //***
   // Extract the data kernel from the image chain:
   //***
   ossimIpt kernel_2d_size (theKernelSize, theKernelSize);
   ossimIpt kernel_ul (theViewPoint - kernel_2d_size/2.0);
   ossimIpt kernel_lr (kernel_ul.x + kernel_2d_size.x - 1,
                       kernel_ul.y + kernel_2d_size.y - 1);
   ossimIrect kernel_rect (kernel_ul, kernel_lr);
   ossimRefPtr<ossimImageData> kernel_data =
      theRemapSource->getTile(kernel_rect);

   //***
   // Fetch the value of the data kernel. Note: this is not necessarily in
   // the same color space as the pixels. It depends on which engine is being
   // utilized:
   //***
   theGridRemapEngine->computeSourceValue(kernel_data, value_vector);

//   delete kernel_data;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbPointSource:: setKernelSize(N)
//
//  Method to set the kernel size used in computing statistics. The kernel
//  will be resized to NxN.
//
//*****************************************************************************
void ossimAtbPointSource::setKernelSize(int side_size)
{
   static const char MODULE[] = "ossimAtbPointSource::setKernelSize()";
   if (traceExec())  CLOG << "entering..." << endl;

   theKernelSize = side_size;
   
   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

 

//*****************************************************************************
// FILE: ossimAtbMatchPoint.cc
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
// DESCRIPTION: Contains implementation of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbMatchPoint.cpp,v 1.3 2001/12/14 19:45:57 okramer Exp $

#include "ossimAtbMatchPoint.h"
#include "ossimAtbPointSource.h"
#include "ossimGridRemapEngine.h"
#include "ossimGridRemapSource.h"

//***
// Define Trace flags for use within this file:
//***
#include "base/common/ossimTrace.h"
static ossimTrace traceExec  ("ossimAtbMatchPoint:exec");
static ossimTrace traceDebug ("ossimAtbMatchPoint:debug");

//*****************************************************************************
//  DESTRUCTOR: ~ossimAtbMatchPoint()
//  
//  Need to delete each instance of an ossimAtbPointSource in thePointSourceList
//  
//*****************************************************************************
ossimAtbMatchPoint::~ossimAtbMatchPoint()
{
   vector<ossimAtbPointSource*>::iterator source = thePointSourceList.begin();
   while (source != thePointSourceList.end())
   {
      delete *source;
      source++;
   }
}
   
//*****************************************************************************
//  METHOD: ossimAtbMatchPoint::addImage()
//
//  Adds a new contribution to the sample set.
//
//*****************************************************************************
void ossimAtbMatchPoint::addImage(ossimGridRemapSource* remapper)
{
   static const char MODULE[] = "ossimAtbMatchPoint::addImage()";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Assure that this image contains the view point corresponding to this
   // matchpoint:
   //***
   if (!remapper->getBoundingRect().pointWithin(theViewPoint))
   {
      if (traceExec())  CLOG << "returning..." << endl;
      return;
   }

   //***
   // Instantiate a point source for this image at this view point and
   // save it in the list:
   //***
   ossimAtbPointSource* point_source = new ossimAtbPointSource(remapper,
                                                               theViewPoint);
   thePointSourceList.push_back(point_source);

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbMatchPoint::assignRemapValues()
//
//  The target parameter value computed given all contributions. The target
//  value is then used to establish the remap parameters for each image at this
//  points location. 
//
//*****************************************************************************
bool ossimAtbMatchPoint::assignRemapValues()
{
   static const char MODULE[] = "ossimAtbMatchPoint::assignRemapValues()";
   if (traceExec())  CLOG << "entering..." << endl;

   //***
   // Determine the number of contributors. We require minimum of two:
   //***
   int num_contributors = thePointSourceList.size();
   if (num_contributors < 2)
      return false;

   //***
   // Hand off the computation of the target pixel to the ATB engine being
   // used. The engine implements the methods for computing targets, as these
   // will vary according to algorithm being used:
   //***
   theGridRemapEngine->assignRemapValues(thePointSourceList);

   if (traceExec())  CLOG << "returning..." << endl;
   return true;
}

   
//*****************************************************************************
//  METHOD:  ossimAtbMatchPoint::setKernelSize(N)
//  
//  Hook to set the size of the kernel used by all point sources in computing
//  their mean pixel value. The kernels will be resized to NxN.
//  
//*****************************************************************************
void ossimAtbMatchPoint::setKernelSize(int side_size)
{
   static const char MODULE[] = "ossimAtbController::setKernelSize(N)";
   if (traceExec())  CLOG << "entering..." << endl;

   vector<ossimAtbPointSource*>::iterator psi = thePointSourceList.begin();
   while (psi != thePointSourceList.end())
      (*psi)->setKernelSize(side_size);

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}


//*****************************************************************************
//  METHOD: ossimAtbMatchPoint::setAtbRemapEngine
//  
//*****************************************************************************
void ossimAtbMatchPoint::setGridRemapEngine(ossimGridRemapEngine* engine)
{
   static const char MODULE[] = "ossimAtbMatchPoint::setAtbRemapEngine";
   if (traceExec())  CLOG << "entering..." << endl;

   theGridRemapEngine = engine;
   
   //***
   // Need to communicate this change of engine to the point sources that use
   // it to arrive at a "source value":
   //***
   vector<ossimAtbPointSource*>::iterator source = thePointSourceList.begin();
   while (source != thePointSourceList.end())
   {
      (*source)->setGridRemapEngine(engine);
      source++;
   }

   if (traceExec())  CLOG << "returning..." << endl;
   return;
}

//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains implementation of class 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbMatchPoint.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/imaging/ossimAtbMatchPoint.h>
#include <ossim/imaging/ossimAtbPointSource.h>
#include <ossim/imaging/ossimGridRemapEngine.h>
#include <ossim/imaging/ossimGridRemapSource.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
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
   thePointSourceList.clear();
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
   ossim_uint32 num_contributors = (ossim_uint32)thePointSourceList.size();
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

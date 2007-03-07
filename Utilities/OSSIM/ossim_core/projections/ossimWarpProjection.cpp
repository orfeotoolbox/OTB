//*****************************************************************************
// FILE: ossimWarpProjection.cc
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
// DESCRIPTION:
//   Contains implementation of class ossimWarpModel. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimWarpProjection.cpp,v 1.11 2005/09/09 19:23:58 gpotts Exp $

#include <projections/ossimWarpProjection.h>
RTTI_DEF1(ossimWarpProjection, "ossimWarpProjection", ossimProjection);

#include <base/misc/ossimQuadTreeWarp.h>
#include <base/misc/ossimAffineTransform.h>
#include <projections/factory/ossimProjectionFactoryRegistry.h>
#include <base/context/ossimNotifyContext.h>

static const char* AFFINE_PREFIX     = "affine.";
static const char* QUADWARP_PREFIX   = "quadwarp.";
static const char* PROJECTION_PREFIX = "projection.";

//***
// Define Trace flags for use within this file:
//***
#include <base/common/ossimTrace.h>
static ossimTrace traceExec  ("ossimWarpProjection:exec");
static ossimTrace traceDebug ("ossimWarpProjection:debug");

//*****************************************************************************
//  CONSTRUCTOR: Default
//  
//*****************************************************************************
ossimWarpProjection::ossimWarpProjection()
   :
      ossimProjection(),
      theClientProjection (NULL),
      theWarpTransform (NULL),
      theAffineTransform (NULL)
{
   theWarpTransform   = new ossimQuadTreeWarp;
   theAffineTransform = new ossimAffineTransform;   
}


//*****************************************************************************
//  CONSTRUCTOR: Primary constructor accepting pointer to the underlying
//               client projection
//  
//*****************************************************************************
ossimWarpProjection::ossimWarpProjection(ossimProjection* client)
   :
      ossimProjection(),
      theClientProjection (client),
      theWarpTransform (NULL),
      theAffineTransform (NULL)
{
   theWarpTransform   = new ossimQuadTreeWarp;
   theAffineTransform = new ossimAffineTransform;
}

//*****************************************************************************
//  CONSTRUCTOR: Accepts geom keywordlist
//*****************************************************************************
ossimWarpProjection::ossimWarpProjection(const ossimKeywordlist& geom_kwl,
                                         const char* prefix)
   :
      ossimProjection(),
      theClientProjection (NULL),
      theWarpTransform (NULL),
      theAffineTransform (NULL)
{
   theClientProjection = ossimProjectionFactoryRegistry::instance()->
                         createProjection(geom_kwl, prefix);

   theWarpTransform = new ossimQuadTreeWarp();
   bool rtn_stat = theWarpTransform->loadState(geom_kwl, prefix);

   theAffineTransform = new ossimAffineTransform();
   rtn_stat &= theAffineTransform->loadState(geom_kwl, prefix);

   if ((!theClientProjection) ||
       (theClientProjection->getErrorStatus()) ||
       (!rtn_stat))
      setErrorStatus(ossimErrorCodes::OSSIM_ERROR);
}
   
//*****************************************************************************
//  DESTRUCTOR
//*****************************************************************************
ossimWarpProjection::~ossimWarpProjection()
{
   if(theClientProjection)
   {
      delete theClientProjection;
      theClientProjection = NULL;
   }
   if(theWarpTransform)
   {
      delete theWarpTransform;
      theWarpTransform = NULL;
   }
   if(theAffineTransform)
   {
      delete theAffineTransform;
      theAffineTransform = NULL;
   }
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::worldToLineSample()
//*****************************************************************************
void ossimWarpProjection::worldToLineSample (const ossimGpt& worldPoint,
                                        ossimDpt&       lineSampPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::worldToLineSample: Entering..." << std::endl;

   if (theClientProjection && theWarpTransform && theAffineTransform)
   {
      theClientProjection->worldToLineSample(worldPoint, lineSampPt);
      theAffineTransform->inverse(lineSampPt);
      theWarpTransform->inverse(lineSampPt);
   }
   else
   {
      lineSampPt.makeNan();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::worldToLineSample: Returning..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::lineSampleToWorld()
//*****************************************************************************
void ossimWarpProjection::lineSampleToWorld(const ossimDpt& lineSampPt,
                                            ossimGpt&       worldPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::lineSampleToWorld: Entering..." << std::endl;

   if (theClientProjection && theWarpTransform && theAffineTransform)
   {
      ossimDpt adjustedPt;
      theWarpTransform->forward(adjustedPt);
      theAffineTransform->forward(lineSampPt, adjustedPt);
      theClientProjection->lineSampleToWorld(adjustedPt, worldPt);
   }
   else
   {
      worldPt.makeNan();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::lineSampleToWorld: Returning..." << std::endl;
}
   
//*****************************************************************************
//  METHOD: ossimWarpProjection::lineSampleToWorld()
//*****************************************************************************
void ossimWarpProjection::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                             const double&   hgt,
                                             ossimGpt&       worldPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::lineSampleHeightToWorld: Entering..." << std::endl;

   if (theClientProjection && theWarpTransform && theAffineTransform)
   {
      ossimDpt adjustedPt;
      theAffineTransform->forward(lineSampPt, adjustedPt);
      theWarpTransform->forward(adjustedPt);
      theClientProjection->lineSampleHeightToWorld(adjustedPt, hgt, worldPt);
   }
   else
   {
      worldPt.makeNan();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::lineSampleHeightToWorld: Returning..." << std::endl;
}
   
//*****************************************************************************
//  METHOD: ossimWarpProjection::print()
//*****************************************************************************
std::ostream& ossimWarpProjection::print(std::ostream& out) const
{
   if (theClientProjection && theWarpTransform && theAffineTransform)
   {
      out <<
         "ossimWarpProjection:\n"
	  << "  Member theClientProjection: ";
      theClientProjection->print(out);
      
      out << "  Member theAffineTransform: "  << *theAffineTransform <<  "\n"
	  << "  Member theWarpTransform: "    << *theWarpTransform << endl;
   }
   else
   {
      out << "ossimWarpProjection -- Not inititialized." << endl;
   }
   return out;
}

std::ostream& operator<<(std::ostream& os, const ossimWarpProjection& m)
{
   return m.print(os);
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::saveState()
//*****************************************************************************
bool ossimWarpProjection::saveState(ossimKeywordlist& kwl,
                                            const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::saveState: entering..." << std::endl;

   ossimString projPrefix     = ossimString(prefix) + PROJECTION_PREFIX;
   ossimString affinePrefix   = ossimString(prefix) + AFFINE_PREFIX;
   ossimString quadwarpPrefix = ossimString(prefix) + QUADWARP_PREFIX;

   if (theClientProjection && theWarpTransform && theAffineTransform)
   {
      theClientProjection->saveState(kwl, projPrefix.c_str());
      theAffineTransform->saveState(kwl, affinePrefix.c_str());
      theWarpTransform->saveState(kwl, quadwarpPrefix.c_str());
   }
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::saveState: returning..." << std::endl;
   return ossimProjection::saveState(kwl, prefix);
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::loadState()
//*****************************************************************************
bool ossimWarpProjection::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::loadState: entering..." << std::endl;

//   bool good_load;
//   int  err_stat;
   bool result = true;
   
   ossimString projPrefix = ossimString(prefix) + PROJECTION_PREFIX;
   ossimString affinePrefix = ossimString(prefix) + AFFINE_PREFIX;
   ossimString quadwarpPrefix = ossimString(prefix) + QUADWARP_PREFIX;

   if(theClientProjection) delete theClientProjection;
   theClientProjection = (ossimProjection*)NULL;

   if (!theWarpTransform)
      theWarpTransform = new ossimQuadTreeWarp();

   if (!theAffineTransform)
      theAffineTransform = new ossimAffineTransform();

   theClientProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, projPrefix.c_str());

   if(!theClientProjection)
     {
       result = false;
     }
   else
     {
       theWarpTransform->loadState(kwl,   quadwarpPrefix.c_str());
       theAffineTransform->loadState(kwl, affinePrefix.c_str());
     }

   return ossimProjection::loadState(kwl, prefix);
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::dup()
//*****************************************************************************
ossimObject* ossimWarpProjection::dup() const
{
   ossimKeywordlist kwl;
   saveState(kwl);
   return new ossimWarpProjection(kwl);
}
   
//*****************************************************************************
//  METHOD: 
//*****************************************************************************
ossimGpt ossimWarpProjection::origin() const
{
   if (theClientProjection)
      return theClientProjection->origin();
   return ossimGpt(0.0, 0.0, 0.0);
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
ossimDpt  ossimWarpProjection::getMetersPerPixel() const
{
   if (theClientProjection)
      return theClientProjection->getMetersPerPixel();
   return ossimDpt(OSSIM_NAN, OSSIM_NAN);
}
      

void ossimWarpProjection::setNewWarpTransform(ossim2dTo2dTransform* warp)
{
   if(warp)
   {
      if(theWarpTransform)
      {
         delete theWarpTransform;
         theWarpTransform = NULL;
      }
      theWarpTransform = warp;
   }
}

void ossimWarpProjection::setNewAffineTransform(ossim2dTo2dTransform* affine)
{
   if(affine)
   {
      if(theAffineTransform)
      {
         delete theAffineTransform;
         theAffineTransform = NULL;
      }
      theAffineTransform = affine;
   }
}

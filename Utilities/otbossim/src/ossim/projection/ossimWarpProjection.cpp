//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer
//
// DESCRIPTION:
//   Contains implementation of class ossimWarpModel. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimWarpProjection.cpp 21214 2012-07-03 16:20:11Z dburken $

#include <ossim/projection/ossimWarpProjection.h>
RTTI_DEF1(ossimWarpProjection, "ossimWarpProjection", ossimProjection);

#include <ossim/base/ossimQuadTreeWarp.h>
#include <ossim/base/ossimAffineTransform.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimNotifyContext.h>

static const char* AFFINE_PREFIX     = "affine.";
static const char* QUADWARP_PREFIX   = "quadwarp.";
static const char* PROJECTION_PREFIX = "projection.";

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimWarpProjection:exec");
static ossimTrace traceDebug ("ossimWarpProjection:debug");

//*****************************************************************************
//  CONSTRUCTOR: Default
//  
//*****************************************************************************
ossimWarpProjection::ossimWarpProjection()
   :
      ossimProjection(),
      theClientProjection (0),
      theWarpTransform (0),
      theAffineTransform (0)
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
      theWarpTransform (0),
      theAffineTransform (0)
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
      theClientProjection (0),
      theWarpTransform (0),
      theAffineTransform (0)
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
   theClientProjection = 0;
   theWarpTransform = 0;
   theAffineTransform = 0;
}

//*****************************************************************************
//  METHOD: ossimWarpProjection::worldToLineSample()
//*****************************************************************************
void ossimWarpProjection::worldToLineSample (const ossimGpt& worldPoint,
                                        ossimDpt&       lineSampPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimWarpProjection::worldToLineSample: Entering..." << std::endl;

   if (theClientProjection.valid() && theWarpTransform.valid() && theAffineTransform.valid())
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

   if (theClientProjection.valid() && theWarpTransform.valid() && theAffineTransform.valid())
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

   if (theClientProjection.valid() && theWarpTransform.valid() && theAffineTransform.valid())
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
   if (theClientProjection.valid() && theWarpTransform.valid() && theAffineTransform.valid())
   {
      out <<
         "ossimWarpProjection:\n"
	  << "  Member theClientProjection: ";
      theClientProjection->print(out);
      
      out << "  Member theAffineTransform: "  << *theAffineTransform <<  "\n"
		  << "  Member theWarpTransform: "    << *theWarpTransform << std::endl;
   }
   else
   {
	   out << "ossimWarpProjection -- Not inititialized." << std::endl;
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

   if (theClientProjection.valid() && theWarpTransform.valid() && theAffineTransform.valid())
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
   // bool result = true;
   
   ossimString projPrefix = ossimString(prefix) + PROJECTION_PREFIX;
   ossimString affinePrefix = ossimString(prefix) + AFFINE_PREFIX;
   ossimString quadwarpPrefix = ossimString(prefix) + QUADWARP_PREFIX;

   theClientProjection = 0;

   if (!theWarpTransform)
      theWarpTransform = new ossimQuadTreeWarp();

   if (!theAffineTransform)
      theAffineTransform = new ossimAffineTransform();

   theClientProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, projPrefix.c_str());

   if ( theClientProjection.valid() )
   {
      theWarpTransform->loadState(kwl,   quadwarpPrefix.c_str());
      theAffineTransform->loadState(kwl, affinePrefix.c_str()); 
   }
   
#if 0 /* set but not used warning... */
   if(!theClientProjection)
   {
      result = false;
   }
   else
   {
      theWarpTransform->loadState(kwl,   quadwarpPrefix.c_str());
      theAffineTransform->loadState(kwl, affinePrefix.c_str());
   }
#endif
   
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
   if (theClientProjection.valid())
      return theClientProjection->origin();
   return ossimGpt(0.0, 0.0, 0.0);
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
ossimDpt  ossimWarpProjection::getMetersPerPixel() const
{
   if (theClientProjection.valid())
      return theClientProjection->getMetersPerPixel();
   return ossimDpt(ossim::nan(), ossim::nan());
}
      

void ossimWarpProjection::setNewWarpTransform(ossim2dTo2dTransform* warp)
{
   if(warp)
   {
      theWarpTransform = warp;
   }
}

void ossimWarpProjection::setNewAffineTransform(ossim2dTo2dTransform* affine)
{
   if(affine)
   {
      theAffineTransform = affine;
   }
}

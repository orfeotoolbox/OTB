//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION:
//   Contains implementation of class ossimAffineModel. This is an
//   implementation of a warping interpolation model.
//
//*****************************************************************************
//  $Id: ossimAffineProjection.cpp 15766 2009-10-20 12:37:09Z gpotts $

#include <ossim/projection/ossimAffineProjection.h>
RTTI_DEF1(ossimAffineProjection, "ossimAffineProjection", ossimProjection);

#include <ossim/base/ossimAffineTransform.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimAffineProjection:exec");
static ossimTrace traceDebug ("ossimAffineProjection:debug");

//*****************************************************************************
//  CONSTRUCTOR: Default
//  
//*****************************************************************************
ossimAffineProjection::ossimAffineProjection()
   :
      ossimProjection(),
      theClientProjection (NULL),
      theAffineTransform (NULL)
{
}


//*****************************************************************************
//  CONSTRUCTOR: Primary constructor accepting pointer to the underlying
//               client projection
//  
//*****************************************************************************
ossimAffineProjection::ossimAffineProjection(ossimProjection* client)
   :
      ossimProjection(),
      theClientProjection (client),
      theAffineTransform (NULL)
{
}

//*****************************************************************************
//  CONSTRUCTOR: Accepts geom keywordlist
//*****************************************************************************
ossimAffineProjection::ossimAffineProjection(const ossimKeywordlist& geom_kwl,
                                         const char* prefix)
   :
      ossimProjection(),
      theClientProjection (NULL),
      theAffineTransform (NULL)
{
   theClientProjection = ossimProjectionFactoryRegistry::instance()->
                         createProjection(geom_kwl, prefix);

   theAffineTransform = new ossimAffineTransform();
   bool rtn_stat = theAffineTransform->loadState(geom_kwl, prefix);

   if (!theClientProjection ||
       theClientProjection->getErrorStatus() || !rtn_stat)
      setErrorStatus(ossimErrorCodes::OSSIM_ERROR);
}
   
//*****************************************************************************
//  DESTRUCTOR
//*****************************************************************************
ossimAffineProjection::~ossimAffineProjection()
{
}

//*****************************************************************************
//  METHOD: ossimAffineProjection::worldToLineSample()
//*****************************************************************************
void
ossimAffineProjection::worldToLineSample (const ossimGpt& worldPoint,
                                          ossimDpt&       lineSampPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::worldToLineSample: Entering..." << std::endl;

   if (theClientProjection.valid() && theAffineTransform.valid())
   {
      theClientProjection->worldToLineSample(worldPoint, lineSampPt);
      theAffineTransform->inverse(lineSampPt);
   }
   else
      lineSampPt.makeNan();
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::worldToLineSample: Returning..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimAffineProjection::lineSampleToWorld()
//*****************************************************************************
void
ossimAffineProjection::lineSampleToWorld(const ossimDpt& lineSampPt,
                                       ossimGpt&       worldPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::lineSampleToWorld: Entering..." << std::endl;

   if (theClientProjection.valid() && theAffineTransform.valid())
   {
      ossimDpt adjustedPt;
      theAffineTransform->forward(lineSampPt, adjustedPt);
      theClientProjection->lineSampleToWorld(adjustedPt, worldPt);
   }
   else
   {
      worldPt.makeNan();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::lineSampleToWorld: Returning..." << std::endl;
}
   
//*****************************************************************************
//  METHOD: ossimAffineProjection::lineSampleToWorld()
//*****************************************************************************
void
ossimAffineProjection::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                               const double&   hgt,
                                               ossimGpt&       worldPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::lineSampleHeightToWorld: Entering..." << std::endl;

   if (theClientProjection.valid() && theAffineTransform.valid())
   {
      ossimDpt adjustedPt;
      theAffineTransform->forward(lineSampPt, adjustedPt);
      theClientProjection->lineSampleHeightToWorld(adjustedPt, hgt, worldPt);
   }
   else
   {
      worldPt.makeNan();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "dEBUG ossimAffineProjection::lineSampleHeightToWorld: Returning..." << std::endl;
}
   
//*****************************************************************************
//  METHOD: ossimAffineProjection::print()
//*****************************************************************************
std::ostream& ossimAffineProjection::print(std::ostream& out) const
{
   
   if (theClientProjection.valid() && theAffineTransform.valid())
   {
      out <<
         "ossimAffineProjection:\n"
         "  Member theClientProjection: ";
      theClientProjection->print(out);
      out << "\n Member theAffineTransform: "
		  << *theAffineTransform << std::endl;
   }
   else
   {
	   out << "ossimAffineProjection -- Not initialized." << std::endl;
   }
   return out;
}

std::ostream& operator<<(std::ostream& os, const ossimAffineProjection& m)
{
   return m.print(os);
}

//*****************************************************************************
//  METHOD: ossimAffineProjection::()
//*****************************************************************************
bool ossimAffineProjection::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::saveState: entering..." << std::endl;

   if (theClientProjection.valid() && theAffineTransform.valid())
   {
      theClientProjection->saveState(kwl, prefix);
      theAffineTransform->saveState(kwl, prefix);
   }

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::saveState: returning..." << std::endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimAffineProjection::()
//*****************************************************************************
bool ossimAffineProjection::loadState(const ossimKeywordlist& kwl,
                                            const char* prefix)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::loadState: entering..." << std::endl;

   bool good_load;
   int  err_stat;
   
   if (!theClientProjection)
   {
      theClientProjection = ossimProjectionFactoryRegistry::instance()->
                            createProjection(kwl, prefix);
      err_stat = theClientProjection->getErrorStatus();
      if (err_stat != ossimErrorCodes::OSSIM_OK)
      {
         return false;
      }
   }
   else
   {
      good_load = theClientProjection->loadState(kwl, prefix);
      if (!good_load)
      {
         return false;
      }
   }

   if (!theAffineTransform)
   {
      theAffineTransform = new ossimAffineTransform();
   }
   good_load = theAffineTransform->loadState(kwl, prefix);
   if (!good_load)
   {
      return false;
   }
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimAffineProjection::loadState: returning..." << std::endl;

   return true;

}

//*****************************************************************************
//  METHOD: ossimAffineProjection::()
//*****************************************************************************
ossimObject* ossimAffineProjection::dup() const
{
   ossimKeywordlist kwl;
   saveState(kwl);
   ossimProjection* duped = new ossimAffineProjection(kwl);
   return duped;
}
   
//*****************************************************************************
//  METHOD: 
//*****************************************************************************
ossimGpt ossimAffineProjection::origin() const
{
   if (theClientProjection.valid())
      return theClientProjection->origin();
   return ossimGpt(0.0, 0.0, 0.0);
}

//*****************************************************************************
//  METHOD: 
//*****************************************************************************
ossimDpt  ossimAffineProjection::getMetersPerPixel() const
{
   if (theClientProjection.valid())
      return theClientProjection->getMetersPerPixel();
   return ossimDpt(ossim::nan(), ossim::nan());
}
      

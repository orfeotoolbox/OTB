//*****************************************************************************
// FILE: ossimImageViewProjectionTransform.cc
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Garrett Potts
//
// DESCRIPTION: Contains declaration of ossimImageViewProjectionTransform.
//    This class provides an image to view transform that utilizes two
//    independent 2D-to-3D projections. Intended for transforming view to
//    geographic "world" space to input image space.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimImageViewProjectionTransform.cpp 11412 2007-07-27 13:56:11Z dburken $
//
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimAdjustableParameterInterface.h>

RTTI_DEF1(ossimImageViewProjectionTransform,
          "ossimImageViewProjectionTransform",
          ossimImageViewTransform);

//***
// Define Trace flags for use within this file:
//***
// #include <ossim/base/ossimTrace.h>
// static ossimTrace traceExec  ("ossimImageViewProjectionTransform:exec");
// static ossimTrace traceDebug ("ossimImageViewProjectionTransform:debug");

//*****************************************************************************
//  CONSTRUCTOR: ossimImageViewProjectionTransform
//  
//*****************************************************************************
ossimImageViewProjectionTransform::ossimImageViewProjectionTransform
(  ossimProjection* imageProjection,
   ossimProjection* viewProjection,
   bool ownsImageProjectionFlag,
   bool ownsViewProjectionFlag)
   :
      ossimImageViewTransform(),
      theImageProjection(imageProjection),
      theViewProjection(viewProjection),
      theOwnsImageProjFlag(ownsImageProjectionFlag),
      theOwnsViewProjFlag(ownsViewProjectionFlag),
      theSameProjection(false),
      theInputMapProjectionFlag(false),
      theOutputMapProjectionFlag(false)
{
   if(!theViewProjection)
   {
//      theViewProjection    = new ossimEquDistCylProjection;
      theOwnsViewProjFlag  = true;
   }
 
   if(!theImageProjection)
   {
      theImageProjection   = new ossimEquDistCylProjection;
      theOwnsImageProjFlag = true;
   }
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimImageViewProjectionTransform
//  
//*****************************************************************************
ossimImageViewProjectionTransform::~ossimImageViewProjectionTransform()
{
   if(theImageProjection && theOwnsImageProjFlag)
   {
      delete theImageProjection;
      theImageProjection = NULL;
   }
   if(theViewProjection && theOwnsViewProjFlag)
   {
      delete theViewProjection;
      theViewProjection = NULL;
   }
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::imageToView
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::imageToView
(   const ossimDpt& imagePoint,
    ossimDpt&       viewPoint) const
{
   if(theImageProjection&&theViewProjection)
   {
      ossimGpt gpt;

//       if(theInputMapProjectionFlag)
// 	{
// 	  ((ossimMapProjection*)theImageProjection)->lineSampleToWorldIterate(imagePoint,
// 									      gpt);
// 	}
//       else
// 	{
      theImageProjection->lineSampleToWorld(imagePoint, gpt);
//	}
      
      if(gpt.isLatNan()||gpt.isLonNan())
      {
         viewPoint.makeNan();
         return;
      }
      theViewProjection->worldToLineSample(gpt, viewPoint);
#if 0
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimImageViewProjectionTransform::imageToView:"
            <<"\n    viewPoint:  "<<viewPoint
            <<"\n    gpt:        "<<gpt
            <<"\n    imagePoint: "<<imagePoint<<std::endl;
         
      }
#endif
   }
}
   
//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::viewToImage
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::viewToImage
(   const ossimDpt& viewPoint,
    ossimDpt&       imagePoint) const
{
   bool transformed = false;
   if(theSameProjection)
   {
      ossimDpt en;
      ossimMapProjection* mapIProj = (ossimMapProjection*)theImageProjection;
      ossimMapProjection* mapVProj = (ossimMapProjection*)theViewProjection;

      if(mapIProj&&mapVProj)
      {
         mapVProj->lineSampleToEastingNorthing(viewPoint, en);
         mapIProj->eastingNorthingToLineSample(en, imagePoint);
         return;
      }
   }
   
   if(!transformed&&theImageProjection&&theViewProjection)
   {
      ossimGpt gpt;
      theViewProjection->lineSampleToWorld(viewPoint, gpt);
      
      if(gpt.isLatNan()||gpt.isLonNan())
      {
         imagePoint.makeNan();
         return;
      }
      theImageProjection->worldToLineSample(gpt, imagePoint);
      
#if 0
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimImageViewProjectionTransform::viewToImage:"
            <<"\n    viewPoint:  "<<viewPoint
            <<"\n    gpt:        "<<gpt
            <<"\n    imagePoint: "<<imagePoint<<std::endl;
      }
#endif
//       if(removeError&&!imagePoint.hasNans()&&!viewPoint.hasNans())
//       {
//          ossimDpt err;
         
// 	 getRoundTripErrorImage(err, ossimIpt(imagePoint));
//          imagePoint+=err;
//          if( fabs(imagePoint.x - (int)imagePoint.x) <= FLT_EPSILON)
//          {
//             imagePoint.x = (int)imagePoint.x;
//          }
//          if( fabs(imagePoint.y - (int)imagePoint.y) <= FLT_EPSILON)
//          {
//             imagePoint.y = (int)imagePoint.y;
//          }
//       }
   }
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::setViewProjection
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::setViewProjection(ossimProjection* viewProjection, bool ownsViewProjection) 
{
   if(theViewProjection &&
      (theViewProjection!=viewProjection) &&
      theOwnsViewProjFlag)
   {
      delete theViewProjection;
      theViewProjection = (ossimProjection*)NULL;
   }
   theOwnsViewProjFlag = ownsViewProjection;
   theViewProjection   = viewProjection;
   if(PTR_CAST(ossimMapProjection,
               theViewProjection))
   {
      theOutputMapProjectionFlag = true;
   }
   else
   {
      theOutputMapProjectionFlag = false;
   }
   checkSameProjection();
}

bool ossimImageViewProjectionTransform::setView(ossimObject* baseObject,
                                                bool ownsTheView)
{
   bool result = true;
   if(baseObject)
   {
      // if not null then we will set it if it is of
      // our type
      ossimProjection* proj = PTR_CAST(ossimProjection, baseObject);
      
      if(proj)
      {
         setViewProjection(proj, ownsTheView);
      }
      else
      {
         result = false;
      }
   }
   else
   {
      // if it's null we will just clear the view out
      setViewProjection((ossimProjection*)NULL, true);
   }

   checkSameProjection();
   return result;
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::setImageProjection
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::setImageProjection(ossimProjection* imageProjection, bool ownsImageProjection)
{
   if(theImageProjection &&
      (theImageProjection!=imageProjection) &&
      theOwnsImageProjFlag)
   {
      delete theImageProjection;
      theImageProjection = (ossimProjection*)NULL;
   }
   theOwnsImageProjFlag = ownsImageProjection;
   theImageProjection = imageProjection;
   if(PTR_CAST(ossimMapProjection,
               theImageProjection))
   {
      theInputMapProjectionFlag = true;
   }
   else
   {
      theInputMapProjectionFlag = false;
   }
   
   checkSameProjection();
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::setViewProjection
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::setViewProjection(const ossimProjection& viewProjection)                                                          
{
   if(theViewProjection && theOwnsViewProjFlag)
   {
      delete theViewProjection;
      theViewProjection = (ossimProjection*)NULL;
   }
   theViewProjection   = (ossimProjection*)viewProjection.dup();
   theOwnsViewProjFlag = true;
   if(PTR_CAST(ossimMapProjection,
               theViewProjection))
   {
      theOutputMapProjectionFlag = true;
   }
   else
   {
      theOutputMapProjectionFlag = false;
   }
   checkSameProjection();
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::setImageProjection
//  
//*****************************************************************************
void ossimImageViewProjectionTransform::setImageProjection(const ossimProjection& imageProjection)
{
   if(theImageProjection && theOwnsImageProjFlag)
   {
      delete theImageProjection;
   }

   theImageProjection = (ossimProjection*)imageProjection.dup();
   if(PTR_CAST(ossimMapProjection,
               theImageProjection))
   {
      theInputMapProjectionFlag = true;
   }
   theOwnsImageProjFlag = true;
   checkSameProjection();
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::print
//*****************************************************************************
std::ostream& ossimImageViewProjectionTransform::print(std::ostream& out)const
{
   if(theImageProjection)
   {
      out << "image projection: " << endl;
      theImageProjection->print(out);
   }
   if(theViewProjection)
   {
      out << "view projection: " << endl;
      theViewProjection->print(out);
   }
   return out;
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::getImageProjection
//*****************************************************************************
ossimProjection* ossimImageViewProjectionTransform::getImageProjection()
{
   return theImageProjection;
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::getViewProjection
//  
//*****************************************************************************
ossimProjection* ossimImageViewProjectionTransform::getViewProjection()
{
   return theViewProjection;
}

ossimDrect ossimImageViewProjectionTransform::getImageToViewBounds(const ossimDrect& imageRect)const
{
   ossimDrect result = ossimImageViewTransform::getImageToViewBounds(imageRect);

   if(result.hasNans()&&theImageProjection&&theViewProjection)
   {
      ossimGeoPolygon viewClip;
      theViewProjection->getGroundClipPoints(viewClip);
      if(viewClip.size())
      {
         std::vector<ossimPolygon> visiblePolygons;
         
         std::vector<ossimGpt> imageGpts(4);
         const ossimDatum* viewDatum = theViewProjection->origin().datum();
         theImageProjection->lineSampleToWorld(imageRect.ul(), imageGpts[0]);
         theImageProjection->lineSampleToWorld(imageRect.ur(), imageGpts[1]);
         theImageProjection->lineSampleToWorld(imageRect.lr(), imageGpts[2]);
         theImageProjection->lineSampleToWorld(imageRect.ll(), imageGpts[3]);

         imageGpts[0].changeDatum(viewDatum);
         imageGpts[1].changeDatum(viewDatum);
         imageGpts[2].changeDatum(viewDatum);
         imageGpts[3].changeDatum(viewDatum);
         
         ossimPolyArea2d viewPolyArea(viewClip.getVertexList());
         ossimPolyArea2d imagePolyArea(imageGpts);
         viewPolyArea &= imagePolyArea;
         viewPolyArea.getVisiblePolygons(visiblePolygons);
         if(visiblePolygons.size())
         {
            
            std::vector<ossimDpt> vpts;
            ossim_uint32 idx = 0;
            for(idx=0; idx<visiblePolygons[0].getNumberOfVertices();++idx)
            {
               ossimDpt tempPt;
               ossimGpt gpt(visiblePolygons[0][idx].lat,
                            visiblePolygons[0][idx].lon,
                            0.0,
                            viewDatum);
               theViewProjection->worldToLineSample(gpt,
                                                    tempPt);
               vpts.push_back(tempPt);
            }
            result = ossimDrect(vpts);
         }
      }
      
   }

   return result;
}

//*****************************************************************************
//  METHOD: ossimImageViewProjectionTransform::loadState
//  
//*****************************************************************************
bool ossimImageViewProjectionTransform::loadState(const ossimKeywordlist& kwl,
                                                  const char* prefix)
{
   ossimString newPrefix = prefix;

   if(theImageProjection)
   {
      delete theImageProjection;
      theImageProjection = NULL;
   }
   if(theViewProjection)
   {
      delete theViewProjection;
      theViewProjection = NULL;
   }

   newPrefix = ossimString(prefix) + "view_proj.";
   theViewProjection
      = ossimProjectionFactoryRegistry::instance()->createProjection(kwl, newPrefix.c_str());

   theImageProjection
      = ossimProjectionFactoryRegistry::instance()->createProjection(kwl,
                                                                     (ossimString(prefix)+"image_proj.").c_str());
   
   theOwnsImageProjFlag = true;   
   theOwnsViewProjFlag = true;
   if(PTR_CAST(ossimMapProjection,
               theImageProjection))
   {
      theInputMapProjectionFlag = true;
   }
   else
   {
      theInputMapProjectionFlag = false;
   }
   if(PTR_CAST(ossimMapProjection,
               theViewProjection))
   {
      theOutputMapProjectionFlag = true;
   }
   else
   {
      theOutputMapProjectionFlag = false;
   }

   return ossimImageViewTransform::loadState(kwl, prefix);
}

bool ossimImageViewProjectionTransform::saveState(ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   if(theViewProjection)
   {
      theViewProjection->saveState(kwl,
                                   (ossimString(prefix) + "view_proj.").c_str());
   }
   ossimAdjustableParameterInterface* adjustablesInterface = PTR_CAST(ossimAdjustableParameterInterface,
                                                                      theImageProjection);
   if(adjustablesInterface)
   {
      if(adjustablesInterface->hasDirtyAdjustments())
      {
         theImageProjection->saveState(kwl,
                                       (ossimString(prefix) + "image_proj.").c_str());            
      }
   }
   kwl.add(prefix,
           "type",
           STATIC_TYPE_NAME(ossimImageViewProjectionTransform),
           true);
   
   return ossimImageViewTransform::saveState(kwl, prefix);
}

void ossimImageViewProjectionTransform::checkSameProjection()
{
   theSameProjection = false;
}

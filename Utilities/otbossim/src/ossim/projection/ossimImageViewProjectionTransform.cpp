//*****************************************************************************
// FILE: ossimImageViewProjectionTransform.cc
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Garrett Potts
//         Oscar Kramer (oscar@krameranalytic.com)
//
// DESCRIPTION: Contains declaration of ossimImageViewProjectionTransform.
//    This class provides an image to view transform that utilizes two
//    independent 2D-to-3D projections. Intended for transforming view to
//    geographic "world" space to input image space.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimImageViewProjectionTransform.cpp 18990 2011-03-01 19:41:50Z gpotts $
//
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimGeoPolygon.h>
#include <ossim/base/ossimPolyArea2d.h>

RTTI_DEF1(ossimImageViewProjectionTransform,
          "ossimImageViewProjectionTransform",
          ossimImageViewTransform);

//*****************************************************************************
//  CONSTRUCTOR: ossimImageViewProjectionTransform
//*****************************************************************************
ossimImageViewProjectionTransform::ossimImageViewProjectionTransform
(  ossimImageGeometry* imageGeometry, ossimImageGeometry* viewGeometry)
:  m_imageGeometry(imageGeometry),
   m_viewGeometry(viewGeometry)
{
}

//*****************************************************************************
//  CONSTRUCTOR: ossimImageViewProjectionTransform
//*****************************************************************************
ossimImageViewProjectionTransform::
ossimImageViewProjectionTransform(const ossimImageViewProjectionTransform& src)
: ossimImageViewTransform(src),
  m_imageGeometry(src.m_imageGeometry),
  m_viewGeometry(src.m_viewGeometry)
{
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimImageViewProjectionTransform
//*****************************************************************************
ossimImageViewProjectionTransform::~ossimImageViewProjectionTransform()
{
}

//*****************************************************************************
//  Workhorse of the object. Converts image-space to view-space.
//*****************************************************************************
void ossimImageViewProjectionTransform::imageToView(const ossimDpt& ip, ossimDpt& vp) const
{
   // Check for same geometries on input and output (this includes NULL geoms):
   if (m_imageGeometry == m_viewGeometry)
   {
      vp = ip;
      return;
   }
   
   // Otherwise we need access to good geoms. Check for a bad geometry object:
   if (!m_imageGeometry || !m_viewGeometry)
   {
      vp.makeNan();
      return;
   }
   
   // Check for same projection on input and output sides to save projection to ground:
   if (m_imageGeometry->getProjection() == m_viewGeometry->getProjection())
   {
      // Check for possible same 2D transforms as well:
      if ( (m_imageGeometry->getTransform() == m_viewGeometry->getTransform()) &&
           (m_imageGeometry->decimationFactor(0) == m_viewGeometry->decimationFactor(0)) )
      {
         vp = ip;
         return;
      }
      
      // Not the same 2D transform, so just perform local-image -> full-image -> local-view:
      ossimDpt fp;
      m_imageGeometry->rnToFull(ip, 0, fp);
      m_viewGeometry->fullToRn(fp, 0, vp);
      return;
   }
   
   //---
   // Completely different left and right side geoms (typical situation).
   // Need to project to ground.
   //---
   ossimGpt gp;
   m_imageGeometry->localToWorld(ip, gp);
   m_viewGeometry->worldToLocal(gp, vp);
   
#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"DEBUG ossimImageViewProjectionTransform::imageToView:"
         <<"\n    ip: "<<ip
         <<"\n    gp: "<<gp
         <<"\n    vp: "<<vp<<std::endl;
      
   }
#endif
}

//*****************************************************************************
//  Other workhorse of the object. Converts view-space to image-space.
//*****************************************************************************
void ossimImageViewProjectionTransform::viewToImage(const ossimDpt& vp, ossimDpt& ip) const
{
   // Check for same geometries on input and output (this includes NULL geoms):
   if (m_imageGeometry == m_viewGeometry)
   {
      ip = vp;
      return;
   }
   
   // Otherwise we need access to good geoms. Check for a bad geometry object:
   if (!m_imageGeometry || !m_viewGeometry)
   {
      ip.makeNan();
      return;
   }
   
   // Check for same projection on input and output sides to save projection to ground:
   if (m_imageGeometry->getProjection() == m_viewGeometry->getProjection())
   {
      // Check for possible same 2D transforms as well:
      if ( (m_imageGeometry->getTransform() == m_viewGeometry->getTransform()) &&
           (m_imageGeometry->decimationFactor(0) == m_viewGeometry->decimationFactor(0)) )
      {
         ip = vp;
         return;
      }
      
      // Not the same 2D transform, so just perform local-image -> full-image -> local-view:
      ossimDpt fp;
      m_viewGeometry->rnToFull(vp, 0, fp);
      m_imageGeometry->fullToRn(fp, 0, ip);
      return;
   }
   
   //---
   // Completely different left and right side geoms (typical situation).
   // Need to project to ground.
   //---
   ossimGpt gp;
   m_viewGeometry->localToWorld(vp, gp);
   m_imageGeometry->worldToLocal(gp, ip);
   
#if 0
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         <<"DEBUG ossimImageViewProjectionTransform::viewToImage:"
         <<"\n    vp: "<<vp
         <<"\n    gp: "<<gp
         <<"\n    ip: "<<ip<<std::endl;
      
   }
#endif
}

//*****************************************************************************
//! OLK: Not sure where this is used, but needed to satisfy ossimViewInterface base class.
//*****************************************************************************
bool ossimImageViewProjectionTransform::setView(ossimObject* baseObject)
{
   ossimImageGeometry* g = dynamic_cast<ossimImageGeometry*>(baseObject);
   bool new_view_set = false;
   if (g)
   {
      m_viewGeometry = g;
      new_view_set = true;
   }
   else
   {
      ossimProjection* proj = dynamic_cast<ossimProjection*>(baseObject);
      if(proj)
      {
         if(m_viewGeometry.valid())
            m_viewGeometry->setProjection(proj);
         else
            m_viewGeometry = new ossimImageGeometry(0, proj);
         
         new_view_set = true;
      }
   }

   return new_view_set;
}

//*****************************************************************************
//  Dumps contents to stream
//*****************************************************************************
std::ostream& ossimImageViewProjectionTransform::print(std::ostream& out)const
{
    out << "ossimImageViewProjectionTransform::print: ..... entered " <<endl;

    if(m_imageGeometry.valid())
    {
        out << "  Input Image (LEFT) Geometry: " << endl;
        m_imageGeometry->print(out);
    }
    else
    {
        out << "  None defined." << endl;
    }
    if(m_viewGeometry.valid())
    {
        out << "Output View (RIGHT) Geometry: " << endl;
        m_viewGeometry->print(out);
    }
    else
    {
        out << "  None defined." << endl;
    }
    return out;
}

//**************************************************************************************************
// Converts the local image space rect into bounding view-space rect
//**************************************************************************************************
ossimDrect ossimImageViewProjectionTransform::getImageToViewBounds(const ossimDrect& imageRect)const
{
    // Let base class try:
   ossimDrect result = ossimImageViewTransform::getImageToViewBounds(imageRect);

   // If not successful, compute using input and output geometries:
   if (result.hasNans() && m_imageGeometry.valid() && m_viewGeometry.valid() &&
       m_imageGeometry->hasProjection() && m_viewGeometry->hasProjection())
   {
      ossimGeoPolygon viewClip;
      m_viewGeometry->getProjection()->getGroundClipPoints(viewClip);
      if(viewClip.size())
      {
         std::vector<ossimGpt> imageGpts(4);
         m_imageGeometry->localToWorld(imageRect.ul(), imageGpts[0]);
         m_imageGeometry->localToWorld(imageRect.ur(), imageGpts[1]);
         m_imageGeometry->localToWorld(imageRect.lr(), imageGpts[2]);
         m_imageGeometry->localToWorld(imageRect.ll(), imageGpts[3]);

         const ossimDatum* viewDatum = m_viewGeometry->getProjection()->origin().datum();
         imageGpts[0].changeDatum(viewDatum);
         imageGpts[1].changeDatum(viewDatum);
         imageGpts[2].changeDatum(viewDatum);
         imageGpts[3].changeDatum(viewDatum);
         
         ossimPolyArea2d viewPolyArea(viewClip.getVertexList());
         ossimPolyArea2d imagePolyArea(imageGpts);
         viewPolyArea &= imagePolyArea;
         std::vector<ossimPolygon> visiblePolygons;
         viewPolyArea.getVisiblePolygons(visiblePolygons);
         if(visiblePolygons.size())
         {
            std::vector<ossimDpt> vpts;
            ossim_uint32 idx = 0;
            for(idx=0; idx<visiblePolygons[0].getNumberOfVertices();++idx)
            {
               ossimDpt tempPt;
               ossimGpt gpt(visiblePolygons[0][idx].lat, visiblePolygons[0][idx].lon, 0.0,  viewDatum);
               m_viewGeometry->worldToLocal(gpt, tempPt);
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
//*****************************************************************************
bool ossimImageViewProjectionTransform::loadState(const ossimKeywordlist& kwl,
                                                  const char* prefix)
{
   bool result = ossimImageViewTransform::loadState(kwl, prefix);
   if(result)
   {
      ossimString imagePrefix = ossimString(prefix)+"image_geometry.";
      ossimString viewPrefix  = ossimString(prefix)+"view_geometry.";
      if(kwl.numberOf(imagePrefix.c_str())>0)
      {
         m_imageGeometry = new ossimImageGeometry();
         m_imageGeometry->loadState(kwl, imagePrefix.c_str());
      }
      if(kwl.numberOf(viewPrefix.c_str())>0)
      {
         m_viewGeometry = new ossimImageGeometry();
         m_viewGeometry->loadState(kwl, viewPrefix.c_str());
      }
      
   }
   
   return result;
}

//**************************************************************************************************
// 
//**************************************************************************************************
bool ossimImageViewProjectionTransform::saveState(ossimKeywordlist& kwl,
                                                  const char* prefix)const
{
   ossimString imagePrefix = ossimString(prefix)+"image_geometry.";
   ossimString viewPrefix  = ossimString(prefix)+"view_geometry.";
   
   if(m_imageGeometry.valid())
   {
      m_imageGeometry->saveState(kwl, imagePrefix.c_str());
   }
   if(m_viewGeometry.valid())
   {
      m_viewGeometry->saveState(kwl, viewPrefix.c_str());
   }
   return ossimImageViewTransform::saveState(kwl, prefix);
}

//**************************************************************************************************
// Returns the GSD of input image.
//**************************************************************************************************
ossimDpt ossimImageViewProjectionTransform::getInputMetersPerPixel() const
{
    ossimDpt result;

    if(m_imageGeometry->hasProjection())
        result = m_imageGeometry->getProjection()->getMetersPerPixel();
    else
        result.makeNan();

    return result;
}

//**************************************************************************************************
// Returns the GSD of the output view.
//**************************************************************************************************
ossimDpt ossimImageViewProjectionTransform::getOutputMetersPerPixel() const
{
    ossimDpt result;

    if(m_viewGeometry->hasProjection())
        result = m_viewGeometry->getProjection()->getMetersPerPixel();
    else
        result.makeNan();

    return result;
}


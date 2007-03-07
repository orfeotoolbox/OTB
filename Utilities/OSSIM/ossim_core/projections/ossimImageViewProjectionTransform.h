//*****************************************************************************
// FILE: ossimImageViewProjectionTransform.h
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
// AUTHOR: Garrett Potts (gpotts@imagelinks.com)
//
// DESCRIPTION: Contains declaration of ossimImageViewProjectionTransform.
//    This class provides an image to view transform that utilizes two
//    independent 2D-to-3D projections. Intended for transforming view to
//    geographic "world" space to input image space.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimImageViewProjectionTransform.h,v 1.14 2005/09/28 19:23:17 gpotts Exp $

#ifndef ossimImageViewProjectionTransform_HEADER
#define ossimImageViewProjectionTransform_HEADER

#include <projections/ossimImageViewTransform.h>
#include <projections/ossimProjection.h>
#ifndef NULL
#include <stddef.h>
#endif

class ossimProjection;
class ossimMapProjection;

class OSSIMDLLEXPORT ossimImageViewProjectionTransform : public ossimImageViewTransform
{
public:
   ossimImageViewProjectionTransform(ossimProjection* imageProjection=NULL,
                                     ossimProjection* viewProjection=NULL,
                                     bool ownsImageProjectionFlag=true,
                                     bool ownsViewProjectionFlag=true);
   
   virtual ~ossimImageViewProjectionTransform();

   virtual bool isValid()const
      {
         return (theImageProjection&&theViewProjection);
      }
  virtual bool isIdentity()const
  {
    if(theImageProjection&&theViewProjection)
      {
	return (*theImageProjection)==(*theViewProjection);
      }
    
    return true;
  }
   virtual void imageToView(const ossimDpt& imagePoint,
                            ossimDpt&       viewPoint)const;
   virtual void viewToImage(const ossimDpt& viewPoint,
                            ossimDpt&       imagePoint)const;

//   virtual void getRoundTripErrorView(ossimDpt& result,
// 				     const ossimDpt& viewPt)const;

//   virtual void getRoundTripErrorImage(ossimDpt& result,
// 				      const ossimDpt& imagePt)const;
   /*!
    * Will not allocate a new projection.  It will just copy
    * the pointer and delete the one it owns if they addresses
    * are different.  It will own the passes in projection.
    */
   void setViewProjection(ossimProjection* viewProjection,
                          bool ownsViewProjection=false);   

   /*!
    * Will allocate a new projection and copy it.
    */
   void setViewProjection(const ossimProjection& viewProjection);

   /*!
    * Will not allocate a new projection.  It will just copy
    * the pointer and delete the one it owns if they addresses
    * are different. Flag indicates whether this becomes owner.
    */
   void setImageProjection(ossimProjection* imageProjection,
                           bool ownsImageProjection=false);

   /*!
    * Will allocate a new projection and copy it.
    */
   void setImageProjection(const ossimProjection& imageProjection);

   virtual std::ostream& print(std::ostream& out) const;
   
   ossimProjection* getImageProjection();
   ossimProjection* getViewProjection();

   virtual bool setView(ossimObject* baseObject,
                        bool ownsTheView = false);
   virtual ossimObject* getView()
      {
         return theViewProjection;
      }
   virtual const ossimObject* getView()const
      {
         return theViewProjection;
      }

   virtual ossimDpt getInputMetersPerPixel()const
      {
         ossimDpt result;

         result.makeNan();

         if(theImageProjection)
         {
            result = theImageProjection->getMetersPerPixel();
         }

         return result;
      }
   virtual ossimDpt getOutputMetersPerPixel()const
      {
         ossimDpt result;

         result.makeNan();

         if(theViewProjection)
         {
            result = theViewProjection->getMetersPerPixel();
         }

         return result;
      }
   virtual ossimDrect getImageToViewBounds(const ossimDrect& imageRect)const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix =0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
protected:
   ossimProjection* theImageProjection;
   ossimProjection* theViewProjection;
   bool             theOwnsImageProjFlag;
   bool             theOwnsViewProjFlag;
   mutable bool     theSameProjection;
   mutable bool     theInputMapProjectionFlag;
   mutable bool     theOutputMapProjectionFlag;
   void checkSameProjection();
//    void findWorldPoint(const ossimDpt& imagePoint,
//                        ossimGpt& worldPt,
//                        ossimMapProjection* proj)const;
   
TYPE_DATA
};

#endif

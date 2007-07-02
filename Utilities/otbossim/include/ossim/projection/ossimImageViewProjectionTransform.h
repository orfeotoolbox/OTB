//*******************************************************************
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimImageViewProjectionTransform.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimImageViewProjectionTransform_HEADER
#define ossimImageViewProjectionTransform_HEADER

#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimProjection.h>
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

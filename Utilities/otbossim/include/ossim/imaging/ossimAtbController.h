//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimAtbController.
//   This is the class for the top-level object controlling the automated
//   tonal balancing (ATB) function. The controller accepts a mosaic.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbController.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimAtbController_HEADER
#define ossimAtbController_HEADER

#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimProcessInterface.h>
#include <vector>

class ossimImageCombiner;
class ossimKeywordlist;
class ossimAtbMatchPoint;
class ossimGridRemapSource;
class ossimGridRemapEngine;
class ossimConnectableContainer;

/*****************************************************************************
 *
 * CLASS: ossimAtbController 
 *
 *****************************************************************************/
class ossimAtbController :
   public ossimConnectableObject,
   public ossimProcessInterface
{
public:
   /*!
    * default Constructor initializes the member combiner pointer to NULL.
    */
   ossimAtbController();
   
   /*!
    * Constructor with Combiner reference. This combiner is used for the ATB.
    */
   ossimAtbController(ossimImageCombiner*   combiner,
                      ossimGridRemapEngine* engine=0);

   /*!
    * Constructor with a specified remap engine.
    */
   ossimAtbController(ossimGridRemapEngine* engine);

   ~ossimAtbController();

  virtual ossimObject* getObject()
  {
    return this;
  }
  virtual const ossimObject* getObject()const
  {
    return this;
  }
   /*!
    * Provide alternate means of initialization in case default constructor
    * used.
    */
   void initializeWithCombiner(ossimImageCombiner*   combiner,
			       ossimGridRemapEngine* engine=0);
   
   /*!
    * Enable an image in the mosaic for the tonal balancing process
    */
   bool enableImage (unsigned int index);

   /*!
    * Disnable an image in the mosaic from the tonal balancing process
    */
   bool disableImage(unsigned int index);

   /*!
    * Locks a particular source for adjustment. A locked image
    * effectively defines the target values for all overlapping imagery. 
    */
   bool lockImage      (unsigned int index);

   /*!
    * Unlock a particular source for adjustment. 
    */
   bool unlockImage    (unsigned int index);

   /*!
    * Implementation of ossimProcessInterface pure virtual method.
    */
   virtual bool execute();
   virtual void abort();

   /*!
    * Hook to set the size of the kernel used by all point sources in computing
    * their mean pixel value. The kernels will be resized to NxN.
    */
   void setKernelSize(int side_size);

   /*!
    * Sets the grid spacing used in gridding the mosaic's bounding rectangle.
    */
   void setGridSpacing(const ossimIpt& spacing);

   /*!
    * Sets the ATB remap engine. This object implements the specific tonal
    * balancing algorithm. The dependency on number of bands and color space,
    * is limited only to this engine. This facilitates the implementation of
    * various grid-based image matching schemes.
    */
   void setGridRemapEngine(ossimGridRemapEngine* engine);
   
   /*!
    * Implementations of ossimStateInterface pure virtuals
    */
   virtual bool saveState(ossimKeywordlist& kwl) const;
   virtual bool loadState(const ossimKeywordlist& kwl);
   
   /*!
    * Implementation of ossimConnectableObject pure virtual.
    */
   virtual bool canConnectMyInputTo(ossim_int32 myInputIndex,
                                    const ossimConnectableObject* object)const;

   
protected:
   void initializeRemappers();

   ossimDrect                       theBoundingRect;
   ossimRefPtr<ossimConnectableContainer>       theContainer;
   ossimGridRemapEngine*            theGridRemapEngine;
   std::vector<ossimRefPtr<ossimAtbMatchPoint> > theMatchPoints;
   ossimIpt                         theGridSpacing;

   TYPE_DATA
};

#endif

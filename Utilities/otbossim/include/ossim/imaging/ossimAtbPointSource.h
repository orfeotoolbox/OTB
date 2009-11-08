//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimAtbPointSource.
//   This object provides the statistics associated with a given point on a
//   given image corresponding to a matchpoint. A matchpoint contains a
//   collection of these point sources, one for each contributing image.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbPointSource.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimAtbPointSource_HEADER
#define ossimAtbPointSource_HEADER

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>

class ossimImageSource;
class ossimImageData;
class ossimGridRemapSource;
class ossimGridRemapEngine;

/*!****************************************************************************
 *
 * CLASS: ossimAtbPointSource 
 *
 *****************************************************************************/
class ossimAtbPointSource : public ossimReferenced
{
public:
   ossimAtbPointSource();
   ossimAtbPointSource(ossimGridRemapSource* remap_source,
                       const ossimDpt&       view_point);


   /*!
    * Sets the pointer of the source of pixels used to compute the stats.
    */
   void setRemapSource(ossimGridRemapSource* image_source);

   /*!
    * Returns the reference to the remapper feeding this source.
    */
   ossimGridRemapSource* getRemapSource() { return theRemapSource.get(); }

   /*!
    * Sets the view coordinates corresponding to this point.
    */
   void setViewPoint(const ossimDpt& view_point);

   /*!
    * Returns the view point corresponding to this point source.
    */
   const ossimDpt& getViewPoint() const { return theViewPoint; }

   /*!
    * Returns the computed value vector corresponding to the region about the
    * view point. The ATB Remap engine performs the actual computation, since
    * the definition of "value" is algorithm dependent (the value may be in a
    * different color space).
    */
   void getSourceValue(void* value);

   /*!
    * Method to set the kernel size used in computing statistics. The kernel
    * will be resized to NxN.
    */
   void setKernelSize(int side_size);

   /*!
    * Sets the ATB remap engine reference owned by the controller. This engine
    * is the only object that understands how to compute remap parameters from
    * pixel data. This permits easily modifying the ATB algorithm without
    * the support classes such as this one.
    */
   void setGridRemapEngine(ossimGridRemapEngine* engine)
      { theGridRemapEngine = engine; }
protected:
   virtual ~ossimAtbPointSource();

private:
   
   ossimRefPtr<ossimGridRemapSource> theRemapSource;
   ossimDpt              theViewPoint;
   ossimRefPtr<ossimGridRemapEngine> theGridRemapEngine;
   int                   theKernelSize;
   bool                  theViewPointIsValid;
};

#endif

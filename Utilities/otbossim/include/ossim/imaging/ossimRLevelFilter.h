//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRLevelFilter.h 17932 2010-08-19 20:34:35Z dburken $
#ifndef ossimRLevelFilter_HEADER
#define ossimRLevelFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

/*!
 * This filter is used to adjust the RLevel.  If you would like to
 * continually change between RLevels or Reslolutions then you can use this
 * filter to do so.  You can also specify whether or not the rLevel
 * passed in from the getTile request is added to this classes current
 * Rlevel.  This is so if you concatenate multiple filters that reduce
 * the RLevels then it will keep reducing.  For example if I have
 * 2 filters that both go to RLevel 1 then it actually will go to
 * 2 since there are 2 decimations happening.  This can be turned off
 * by specifying the Rlevel not to be additive.
 */
class OSSIMDLLEXPORT ossimRLevelFilter : public ossimImageSourceFilter
{
public:

   /** @brief default constructor */
   ossimRLevelFilter();


   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;

   //! Returns a pointer reference to the active image geometry at this filter. The input source
   //! geometry is modified, so we need to maintain our own geometry object as a data member.
   ossimRefPtr<ossimImageGeometry> getImageGeometry();

   virtual void setCurrentRLevel(ossim_uint32 rlevel);

   virtual ossim_uint32 getCurrentRLevel()const;

   /*!
    * Get/set of the data member "theOverrideGeometryFlag".
    * If set this will override the "getImageGeometry" method and adjust
    * the getMetersPerPixel or the getDecimalDegreesPerPixel.
    * Default behavior is to not override method.
    */
   virtual bool getOverrideGeometryFlag() const;
   virtual void setOverrideGeometryFlag(bool override);

   /**
    * @brief Returns the bounding rectangle.
    *
    * @param resLevel This argument is only passed onto the input connection
    * if this filter is disabled.  @see disableSource().
    *
    * To get the bounding rectangle of an rlevel when source is enabled do:
    * myRLevelFilter->setCurrentRLevel(level);
    * myRLevelFilter->getBoundingRect();
    *
    * @return Rectangle of the current rlevel if enabled; else, the rectangle
    * of the input connection for resLevel.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   /** @brief virtual destructor. */
   virtual ~ossimRLevelFilter();
   
   /*!
    * Initializes result with the sum of decimations from rlevel one to
    * theCurrentRLevel.
    */
   void getSummedDecimation(ossimDpt& result) const;
   
   //! If this object is maintaining an ossimImageGeometry, this method needs to be called after 
   //! a scale change so that the geometry's projection is modified accordingly.
   void updateGeometry();

   ossim_uint32 theCurrentRLevel;
   bool         theOverrideGeometryFlag;
   ossimRefPtr<ossimImageGeometry> m_ScaledGeometry; //!< The input image geometry, altered by the scale

TYPE_DATA
};

#endif

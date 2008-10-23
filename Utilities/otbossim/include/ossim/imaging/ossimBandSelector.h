//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt.
// 
// Author:  Garrett Potts
//
// Description:  Contains class declaration for ossimBandSelector.
// 
//*******************************************************************
//  $Id: ossimBandSelector.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimBandSelector_HEADER
#define ossimBandSelector_HEADER

#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimImageData;

class OSSIM_DLL ossimBandSelector : public ossimImageSourceFilter
{
   
public:
   enum ossimBandSelectorWithinRangeFlagState
   {
      ossimBandSelectorWithinRangeFlagState_NOT_SET      = 0,
      ossimBandSelectorWithinRangeFlagState_IN_RANGE     = 1,
      ossimBandSelectorWithinRangeFlagState_OUT_OF_RANGE = 2,

   };
   ossimBandSelector();
   
   virtual ~ossimBandSelector();
   
   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   /**
    * Initializes bandList to the zero based order of output bands.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;

   /**
    * Sets the output band list.
    *
    * @param outputBandList List of zero based bands.
    *
    * @note The input connection should be established prior to calling this.
    *
    * @note Bands can be duplicated.  So if you have a single band input you
    * can do a band list like "1, 1, 1".   Any band within the list must be
    * available from the input.  So if you have a 3 band input bands must be
    * constrained to zero based bands "0, 1, or 2".
    *
    * @note Calling this method does not enable filter so callers should do:
    * theBandSelector->enableSource();
    * theBandSelector->setOutputBandList(bandList);
    */
   virtual void setOutputBandList(const vector<ossim_uint32>& outputBandList);
   
   /**
    * Returns the number of bands in a tile returned from this TileSource.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   virtual void initialize();
   
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
   /**
    *   Override base class so that a disableSource event does not
    *   reinitialize the object and enable itself.
    */
   //virtual void propertyEvent(ossimPropertyEvent& event);
   //virtual void refreshEvent(ossimRefreshEvent& event);   

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;						  
						  
   
private:

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();

   /**
    * @return true if theInputConnection->getOutputBandList is equal to
    * this->theOutputBandList, false if not.
    */
   bool isOrderedCorrectly() const;

   /**
    * @return true if all bands in theOutputBandList are less than the
    * number of input's bands.
    */
   bool outputBandsWithinInputRange() const;
   
   
   ossimRefPtr<ossimImageData> theTile;
   vector<ossim_uint32>        theOutputBandList;
   mutable ossimBandSelectorWithinRangeFlagState theWithinRangeFlag;

TYPE_DATA
};

#endif

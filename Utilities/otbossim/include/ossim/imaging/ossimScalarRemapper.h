//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Contains class declartion for ossimScalarRemapper.
// This class is used to remap image data from one scalar type to another.
//
//*******************************************************************
//  $Id: ossimScalarRemapper.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimScalarRemapper_HEADER
#define ossimScalarRemapper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimScalarRemapper : public ossimImageSourceFilter
{
public:
   ossimScalarRemapper();
   ossimScalarRemapper(ossimImageSource* inputSource,
                       ossimScalarType outputScalarType);

   virtual void initialize();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);

   /**
    *  Returns the output pixel type of the tile source.  This override the
    *  base class since it simply returns it's input scalar type which is
    *  not correct with this object.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    *  Returns the output pixel type of the tile source as a string.
    */
   virtual ossimString getOutputScalarTypeString() const;
   
   /**
    *  Sets the output scalar type.
    */
   virtual void setOutputScalarType(ossimScalarType scalarType);
   virtual void setOutputScalarType(ossimString scalarType);
     
   /**
    *  Sets the current resolution level.  Returns true on success, false
    *  on error.
    */
   virtual double getNullPixelValue(ossim_uint32 band)  const;
   virtual double getMinPixelValue(ossim_uint32 band=0) const;
   virtual double getMaxPixelValue(ossim_uint32 band=0) const;

   /**
    *  Method to save the state of an object to a keyword list.
    *  Return true if ok or false on error.
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
   virtual void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);   

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
protected:
   virtual ~ossimScalarRemapper();

   /**
    *  Called on first getTile, will initialize all data needed.
    */
   void allocate();

   /**
    *  Deletes allocated memory.  Used by both allocate and destructor.
    */
   void destroy();
   
   double*                     theNormBuf;
   ossimRefPtr<ossimImageData> theTile;
   ossimScalarType             theOutputScalarType;

   bool                        theByPassFlag;
TYPE_DATA
};

#endif /* #ifndef ossimScalarRemapper_HEADER */

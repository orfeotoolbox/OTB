//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimFftFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimFftFilter_HEADER
#define ossimFftFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimScalarRemapper;

class ossimFftFilter : public ossimImageSourceFilter
{
public:
   enum ossimFftFilterDirectionType
      {
         ossimFftFilterDirectionType_FORWARD = 0,
         ossimFftFilterDirectionType_INVERSE
      };
   ossimFftFilter(ossimObject* owner=NULL);
   ossimFftFilter(ossimImageSource* inputSource);
   ossimFftFilter(ossimObject* owner,
                  ossimImageSource* inputSource);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                   ossim_uint32 resLevel=0);
   virtual void initialize();
   virtual ossim_uint32 getNumberOfOutputBands() const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
  /*!
   * Returns the max pixel of the band.
   */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
  /*!
   * Each band has a null pixel associated with it.  The null pixel 
   * represents an invalid value.
   */ 
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   
   virtual ossimScalarType getOutputScalarType() const;
   
   void setForward();
   void setInverse();
   ossimString getDirectionTypeAsString()const;
   void setDirectionType(const ossimString& directionType);
   void setDirectionType(ossimFftFilterDirectionType directionType);
      
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
protected:
   virtual ~ossimFftFilter();
   ossimRefPtr<ossimImageData> theTile;
   ossimFftFilterDirectionType theDirectionType;
   ossimRefPtr<ossimScalarRemapper>        theScalarRemapper;
   template <class T>
   void runFft(T dummy,
               ossimRefPtr<ossimImageData>& input,
               ossimRefPtr<ossimImageData>& output);

   template <class T>
   void fillMatrixForward(T *realPart,
                          T nullPix,
                          NEWMAT::Matrix& real,
                          NEWMAT::Matrix& img)const;
   template <class T>
   void fillMatrixInverse(T *realPart,
                          T *imgPart,
                          NEWMAT::Matrix& real,
                          NEWMAT::Matrix& img)const;


TYPE_DATA
};

#endif

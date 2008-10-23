//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimScaleFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimScaleFilter_HEADER
#define ossimScaleFilter_HEADER
#include <vector>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/imaging/ossimFilter.h>

/*!
 */
class ossimScaleFilter : public ossimImageSourceFilter
{
public:
   enum ossimScaleFilterType
   {
      ossimScaleFilter_NEAREST_NEIGHBOR = 0,
      ossimScaleFilter_BOX              = 1,
      ossimScaleFilter_GAUSSIAN         = 2,
      ossimScaleFilter_CUBIC            = 3,
      ossimScaleFilter_HANNING          = 4,
      ossimScaleFilter_HAMMING          = 5,
      ossimScaleFilter_LANCZOS          = 6,
      ossimScaleFilter_MITCHELL         = 7,
      ossimScaleFilter_CATROM           = 8,
      ossimScaleFilter_BLACKMAN         = 9,
      ossimScaleFilter_BLACKMAN_SINC    = 10,
      ossimScaleFilter_BLACKMAN_BESSEL  = 11,
      ossimScaleFilter_QUADRATIC        = 12,
      ossimScaleFilter_TRIANGLE         = 13,
      ossimScaleFilter_HERMITE          = 14
   };
   
   ossimScaleFilter();
   ossimScaleFilter(ossimImageSource* inputSource,
                    const ossimDpt& scaleFactor);
   virtual ~ossimScaleFilter();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                   ossim_uint32 resLevel=0);
   
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix);
   virtual void initialize();
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const
      {
         result.makeNan();

         if(resLevel == 0)
         {
            result.x = 1.0;
            result.y = 1.0;
         }
      }
   virtual void getDecimationFactors(vector<ossimDpt>& decimations)const
      {
         decimations.push_back(ossimDpt(1.0, 1.0));
      }
   virtual ossim_uint32 getNumberOfDecimationLevels()const
      {
         // only full res output since we are scaling
         //
         return 1;
      }
   void setFilterType(ossimScaleFilterType filterType);
   void setFilterType(ossimScaleFilterType minifyFilterType,
                      ossimScaleFilterType magnifyFilterType);
   
   void setFilterType(const ossimString& minifyType,
                      const ossimString& magnifyType)
      {
         setFilterType(getFilterType(minifyType),
                       getFilterType(magnifyType));
      }
   void setMinifyFilterType(const ossimString& minifyType)
      {
         setMinifyFilterType(getFilterType(minifyType));
      }
   void setMagnifyFilterType(const ossimString& magnifyType)
      {
         setMagnifyFilterType(getFilterType(magnifyType));
      }
   void setMinifyFilterType(ossimScaleFilterType filterType)
      {
         setFilterType(filterType,
                       theMagnifyFilterType);
      }
   void setMagnifyFilterType(ossimScaleFilterType filterType)
      {
         setFilterType(theMinifyFilterType,filterType);
      }
   ossimString getMinifyFilterTypeAsString()const
      {
         return getFilterTypeAsString(theMinifyFilterType);
      }
   ossimString getMagnifyFilterTypeAsString()const
      {
         return getFilterTypeAsString(theMagnifyFilterType);
      }
   void setScaleFactor(const ossimDpt& scale);
   void setBlurFactor(ossim_float64 blur)
      {
         theBlurFactor = blur;
      }
   ossim_float64 getBlurFactor()const
      {
         return theBlurFactor;
      }
   const ossimDpt& getScaleFactor()const
      {
         return theScaleFactor;
      }
   /*!
    * Saves the state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   /*!
    * Loads the state of this object.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
protected:
   void allocate();
   
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   ossimFilter*                theMinifyFilter;
   ossimFilter*                theMagnifyFilter;
   ossimScaleFilterType        theMinifyFilterType;
   ossimScaleFilterType        theMagnifyFilterType;
   ossimDpt                    theScaleFactor;
   ossimDpt                    theInverseScaleFactor;
   ossimIpt                    theTileSize;
   ossimIrect                  theInputRect;
   ossim_float64               theBlurFactor;
   
   template <class T>
   void runFilterTemplate(T dummy,
                          const ossimIrect& imageRect,
                          const ossimIrect& viewRect);
   template <class T>
   void runHorizontalFilterTemplate(T dummy,
                                    const ossimRefPtr<ossimImageData>& input,
                                    ossimRefPtr<ossimImageData>& output);
   template <class T>
   void runVerticalFilterTemplate(T dummy,
                                  const ossimRefPtr<ossimImageData>& input,
                                  ossimRefPtr<ossimImageData>& output);
    void runFilter(const ossimIrect& imageRect,
                   const ossimIrect& viewRect);

   void getSupport(double& x, double& y);
   const ossimFilter* getHorizontalFilter()const;
   const ossimFilter* getVerticalFilter()const;
   
   ossimString getFilterTypeAsString(ossimScaleFilterType type)const;
   ossimScaleFilterType getFilterType(const ossimString& type)const;
   
   ossimIrect scaleRect(const ossimIrect input,
                        const ossimDpt& scaleFactor)const;
   ossimFilter* createNewFilter(ossimScaleFilterType filterType,
                                ossimScaleFilterType& result);
TYPE_DATA
};

#endif

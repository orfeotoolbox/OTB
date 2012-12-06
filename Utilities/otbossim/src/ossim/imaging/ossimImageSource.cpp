//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSource.cpp 21637 2012-09-06 21:17:57Z dburken $

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimKeywordNames.h>

RTTI_DEF1(ossimImageSource,
          "ossimImageSource" ,
          ossimSource)

ossimImageSource::ossimImageSource(ossimObject* owner)
   :ossimSource(owner, 0,0,false,false)
{
}

ossimImageSource::ossimImageSource(ossimObject* owner,
                                   ossim_uint32 inputListSize,
                                   ossim_uint32 outputListSize,
                                   bool inputListIsFixedFlag,
                                   bool outputListIsFixedFlag)
   :
      ossimSource(owner, inputListSize, outputListSize,
                  inputListIsFixedFlag, outputListIsFixedFlag)
{
}

ossimImageSource::~ossimImageSource()
{
}

ossimRefPtr<ossimImageData> ossimImageSource::getTile(const ossimIpt& origin,
                                                      ossim_uint32 resLevel)
{
   ossimIrect tileRect(origin.x,
                       origin.y,
                       origin.x + getTileWidth()  - 1,
                       origin.y + getTileHeight() - 1);
   
   return getTile(tileRect, resLevel);
}

ossimRefPtr<ossimImageData> ossimImageSource::getTile(const ossimIrect& rect,
                                                      ossim_uint32 resLevel)
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                      getInput(0));
   if(inter)
   {
      return inter->getTile(rect, resLevel);
   }
   return NULL;
}

bool ossimImageSource::getTile(ossimImageData* result, ossim_uint32 resLevel)
{
   bool status = true;
   
   if (result)
   {
      result->ref();
      
      ossimIrect tileRect = result->getImageRectangle();
      
      ossimRefPtr<ossimImageData> id = getTile(tileRect, resLevel);
      if (id.valid())
      {
         *result = *(id.get());
      }
      else
      {
         status = false;
      }
      result->unref();
   }

   return status;
}

void ossimImageSource::getDecimationFactor(ossim_uint32 resLevel,
                                           ossimDpt& result)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      inter->getDecimationFactor(resLevel, result);
   }
}

void ossimImageSource::getDecimationFactors(std::vector<ossimDpt>& decimations)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      inter->getDecimationFactors(decimations);
   }
}

ossim_uint32 ossimImageSource::getNumberOfDecimationLevels() const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getNumberOfDecimationLevels();
   }
   return 0;
}

ossimScalarType ossimImageSource::getOutputScalarType() const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getOutputScalarType();
   }
   
   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimImageSource::getTileWidth()  const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getTileWidth();
   }
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   
   return tileSize.x;
}

ossim_uint32 ossimImageSource::getTileHeight() const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if(inter)
   {
      return inter->getTileHeight();
   }
   
   ossimIpt tileSize;
   ossim::defaultTileSize(tileSize);
   
   return tileSize.y;
}

ossimIrect ossimImageSource::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if(inter)
   {
      return inter->getBoundingRect(resLevel);
   }
   ossimIrect rect;
   rect.makeNan();
   return rect;
}

void ossimImageSource::getBoundingRect(ossimIrect& rect, ossim_uint32 resLevel) const
{
   rect = getBoundingRect( resLevel );
}

bool ossimImageSource::saveState(ossimKeywordlist& kwl,
                                 const char* prefix)const
{
   return ossimSource::saveState(kwl, prefix);
}

bool ossimImageSource::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   return ossimSource::loadState(kwl, prefix);
}

//*****************************************************************************
//  METHOD: ossimImageSource::getValidImageVertices()
//*****************************************************************************
void ossimImageSource::getValidImageVertices(std::vector<ossimIpt>& validVertices,
                                             ossimVertexOrdering ordering,
                                             ossim_uint32 resLevel)const
{
  ossimImageSource* inter = PTR_CAST(ossimImageSource,
					      getInput(0));
  if(inter)
    {
      return inter->getValidImageVertices(validVertices,
					  ordering,
                                          resLevel);
    }
   ossimIrect boundingRect = getBoundingRect(resLevel);
   validVertices.clear();
   
   if(ordering == OSSIM_CLOCKWISE_ORDER)
   {
      validVertices.push_back(boundingRect.ul());
      validVertices.push_back(boundingRect.ur());
      validVertices.push_back(boundingRect.lr());
      validVertices.push_back(boundingRect.ll());
   }
   else
   {
      validVertices.push_back(boundingRect.ul());
      validVertices.push_back(boundingRect.ll());
      validVertices.push_back(boundingRect.lr());
      validVertices.push_back(boundingRect.ur());
   }
}

double ossimImageSource::getNullPixelValue(ossim_uint32 band)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getNullPixelValue(band);
   }
   
   return ossim::defaultNull(getOutputScalarType());
}

double ossimImageSource::getMinPixelValue(ossim_uint32 band)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getMinPixelValue(band);
   }
   return ossim::defaultMin(getOutputScalarType());
}

double ossimImageSource::getMaxPixelValue(ossim_uint32 band)const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getMaxPixelValue(band);
   }
   
   return ossim::defaultMax(getOutputScalarType());
}

//**************************************************************************************************
// Default implementation returns the image geometry object associated with the first input source 
// (if any) connected to this source, or NULL.
//**************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimImageSource::getImageGeometry()
{
   ossimRefPtr<ossimImageGeometry> result = 0;
   if ( getInput(0) )
   {
      ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
      if( inter )
      {
         result = inter->getImageGeometry();
      }
   }
   return result;
}

//**************************************************************************************************
//! Default implementation sets geometry of the first input to the geometry specified.
//**************************************************************************************************
void ossimImageSource::setImageGeometry(const ossimImageGeometry* geom)
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if (inter)
   {
      inter->setImageGeometry(geom);
   }
}

void ossimImageSource::saveImageGeometry() const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if (inter)
   {
      inter->saveImageGeometry();
   }
}

void ossimImageSource::saveImageGeometry(const ossimFilename& geometry_file) const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if (inter)
   {
      inter->saveImageGeometry(geometry_file);
   }
}

void ossimImageSource::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   const ossim_uint32 INPUT_BANDS = getNumberOfInputBands();
   if ( INPUT_BANDS )
   {
      bandList.resize( INPUT_BANDS );
      for ( ossim_uint32 band = 0; band < INPUT_BANDS; ++band )
      {
         bandList[band] = band;
      }
   }
   else
   {
      bandList.clear();
   }
}

ossim_uint32 ossimImageSource::getNumberOfOutputBands() const
{
   return getNumberOfInputBands();
}

ossimRefPtr<ossimProperty> ossimImageSource::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = ossimSource::getProperty(name);

   ossimString tempName = name;
   if(result.valid())
   {
      if(tempName.downcase() == ossimKeywordNames::ENABLED_KW)
      {
         result->clearChangeType();
         
         // we will at least say its a radiometric change
         //
         result->setFullRefreshBit();
      }
   }

   return result;
}

void ossimImageSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimSource::setProperty(property);
}

void ossimImageSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimSource::getPropertyNames(propertyNames);
}

bool ossimImageSource::isIndexedData() const
{
   bool result = false;
   ossimImageSource* inter = PTR_CAST(ossimImageSource, getInput(0));
   if (inter)
   {
      result = inter->isIndexedData();
   }
   return result;
}

// Protected to hide from use...
ossimImageSource::ossimImageSource (const ossimImageSource& /* rhs */)
   :ossimSource() 
{}

// Protected to hide from use...
const ossimImageSource& ossimImageSource::operator=(const ossimImageSource&)
{
   return *this;
}

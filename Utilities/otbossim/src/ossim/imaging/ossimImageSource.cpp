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
// $Id: ossimImageSource.cpp 13312 2008-07-27 01:26:52Z gpotts $

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageData.h>
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
   ossimIrect tile_rect(origin.x,
                        origin.y,
                        origin.x + getTileWidth()  - 1,
                        origin.y + getTileHeight() - 1);
   
   return getTile(tile_rect, resLevel);
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
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
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
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if(inter)
   {
      return inter->getBoundingRect(resLevel);
   }
   ossimIrect rect;
   rect.makeNan();
   return rect;
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


bool ossimImageSource::getImageGeometry(ossimKeywordlist& kwl,
                                        const char* prefix)
{
   int i = 0;
   for(i = 0; i < (int)getNumberOfInputs(); ++i)
   {
      ossimImageSource* interface = PTR_CAST(ossimImageSource,
                                                      getInput(i));
      if(interface)
      {
         return interface->getImageGeometry(kwl, prefix);
      }
   }
   
   return false;
}

void ossimImageSource::setImageGeometry(const ossimKeywordlist& kwl)
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if (inter)
   {
      setImageGeometry(kwl);
   }
}

void ossimImageSource::saveImageGeometry() const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if (inter)
   {
      saveImageGeometry();
   }
}

void ossimImageSource::saveImageGeometry(const ossimFilename& geometry_file) const
{
   ossimImageSource* inter = PTR_CAST(ossimImageSource,
                                               getInput(0));
   if (inter)
   {
      saveImageGeometry(geometry_file);
   }
}

void ossimImageSource::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   bandList.clear();
   const ossim_uint32 BANDS = getNumberOfOutputBands();
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      bandList.push_back(band);
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

// Protected to hide from use...
ossimImageSource::ossimImageSource (const ossimImageSource& rhs)
   :ossimSource() 
{}

// Protected to hide from use...
const ossimImageSource& ossimImageSource::operator=(const ossimImageSource&)
{
   return *this;
}

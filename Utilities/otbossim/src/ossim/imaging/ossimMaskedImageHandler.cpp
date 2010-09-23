//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Mingjie Su
//
// Description:
//
// Contains class implementation for the class "ossimMaskedImageHandler".
//
//*******************************************************************
//  $Id: ossimMaskedImageHandler.cpp 1242 2010-08-06 20:37:21Z david.burken $

#include <ossim/imaging/ossimMaskedImageHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimMaskTileSource.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimMaskedImageHandler, "ossimMaskedImageHandler", ossimImageHandler);

static ossimTrace traceDebug("ossimMaskedImageHandler:debug");

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimMaskedImageHandler::ossimMaskedImageHandler()
   : ossimImageHandler(),
   m_inputImageSource(0),
   m_maskImageSource(0),
   m_maskFilter(0)
{
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimMaskedImageHandler::~ossimMaskedImageHandler()
{
  close();
}

void ossimMaskedImageHandler::close()
{
  if (m_maskImageSource.valid())
  {
    m_maskImageSource = 0;
  }

  if (m_maskFilter.valid())
  {
    m_maskFilter = 0;
  }
}

//*******************************************************************
// Public Method:
//*******************************************************************
bool ossimMaskedImageHandler::open()
{
  bool result = false;

  if (!isSupportedExtensions())
  {
    return false;
  }

  if (isOpen())
  {
    close();
  }
 
  if (m_inputImageSource != 0)
  {
    m_maskImageSource = new ossimMaskTileSource;
    m_maskImageSource->setFilename(theImageFile);
    m_maskImageSource->setImageBoundingRect(m_inputImageSource->getImageRectangle(1));

    if (m_maskImageSource->open())
    {
      m_maskFilter = new ossimMaskFilter(m_inputImageSource.get(), m_maskImageSource.get());
      result = true;
    }
  }
  else
  {
    result = false;
  }

  return result;
}

bool ossimMaskedImageHandler::open(const ossimFilename& imageFile)
{
  theImageFile = imageFile;
  if (theImageFile.exists())
  {
    return open();
  }
  return false;
}

bool ossimMaskedImageHandler::isOpen()const
{
  if (m_maskFilter != 0 && m_maskImageSource != 0)
  {
    return true;
  }
  return false;
}

bool ossimMaskedImageHandler::isSupportedExtensions()
{
  ossimString ext = theImageFile.ext();
 
  if (ext.downcase() == "mask")
  {
    return true;
  }
  return false;
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimMaskedImageHandler::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   if(m_maskFilter != 0)
   {
      m_maskFilter->saveState(kwl, prefix);
   }
   return ossimImageHandler::saveState(kwl, prefix);
}

//*******************************************************************
// Public method:
//*******************************************************************
bool ossimMaskedImageHandler::loadState(const ossimKeywordlist& kwl,
                                       const char* prefix)
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->loadState(kwl, prefix);
  }

  if (ossimImageHandler::loadState(kwl, prefix) )
  {
    return open();
  }

  return false;
}

ossim_uint32 ossimMaskedImageHandler::getImageTileWidth() const
{
  if (m_inputImageSource != 0)
  {
     return m_inputImageSource->getImageTileWidth();
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getImageTileHeight() const
{
  if (m_inputImageSource != 0)
  {
     return m_inputImageSource->getImageTileHeight();
  }
  return 0;
}

ossimRefPtr<ossimImageGeometry> ossimMaskedImageHandler::getImageGeometry()
{
  if (m_inputImageSource != 0)
  {
     theGeometry = m_inputImageSource->getImageGeometry();
  }
  return theGeometry;  
}

ossimRefPtr<ossimImageData> ossimMaskedImageHandler::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   if (resLevel > 0)
   {
      return m_maskFilter->getTile(tileRect, resLevel);
   }
   return m_inputImageSource->getTile(tileRect, resLevel);
}

ossim_uint32 ossimMaskedImageHandler::getNumberOfInputBands() const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getNumberOfOutputBands();
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getNumberOfOutputBands() const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getNumberOfOutputBands();
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getNumberOfLines(ossim_uint32 reduced_res_level) const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getBoundingRect(reduced_res_level).height();
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getNumberOfSamples(ossim_uint32 reduced_res_level) const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getBoundingRect(reduced_res_level).width();
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getNumberOfDecimationLevels() const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getNumberOfDecimationLevels();
  }
  return 0;
}

ossimIrect ossimMaskedImageHandler::getImageRectangle(ossim_uint32 reduced_res_level) const
{
  if (m_inputImageSource != 0)
  {
    return m_inputImageSource->getImageRectangle(reduced_res_level);
  }
  return ossimIrect(0,0,0,0);
}

ossimScalarType ossimMaskedImageHandler::getOutputScalarType() const
{
   if (m_maskFilter != 0)
   {
      return m_maskFilter->getOutputScalarType();
   }

   return OSSIM_SCALAR_UNKNOWN;
}

ossim_uint32 ossimMaskedImageHandler::getTileWidth() const
{
  if (m_maskFilter != 0)
  {
     return m_maskFilter->getTileWidth();  
  }
  return 0;
}

ossim_uint32 ossimMaskedImageHandler::getTileHeight() const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getTileHeight();  
  }
  return 0;
}

double ossimMaskedImageHandler::getNullPixelValue(ossim_uint32 band)const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getNullPixelValue(band);
  }
  return 0;
}

double ossimMaskedImageHandler::getMinPixelValue(ossim_uint32 band)const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getMinPixelValue(band);
  }
  return 0;
}

double ossimMaskedImageHandler::getMaxPixelValue(ossim_uint32 band)const
{
  if (m_maskFilter != 0)
  {
    return m_maskFilter->getMaxPixelValue(band);
  }
  return 0;
}

void ossimMaskedImageHandler::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(m_maskFilter != 0)
   {
     m_maskFilter->setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimMaskedImageHandler::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;

   if(m_maskFilter != 0)
   {
     result = m_maskFilter->getProperty(name);
   }
   return result;
}

void ossimMaskedImageHandler::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   if(m_maskFilter != 0)
   {
     m_maskFilter->getPropertyNames(propertyNames); 
   }
}

void ossimMaskedImageHandler::setInputImageSource(ossimRefPtr<ossimImageHandler> inputImageSource)
{
  if (m_inputImageSource.valid())
  {
    m_inputImageSource = 0;
  }
  m_inputImageSource = inputImageSource;
}

ossim_uint32 ossimMaskedImageHandler::getCurrentEntry()const
{
  if (m_inputImageSource != 0)
  {
    return m_inputImageSource->getCurrentEntry();
  }
  return 0;
}

bool ossimMaskedImageHandler::setCurrentEntry(ossim_uint32 entryIdx)
{
  if (m_inputImageSource != 0)
  {
    return m_inputImageSource->setCurrentEntry(entryIdx);
  }
  return false;
}

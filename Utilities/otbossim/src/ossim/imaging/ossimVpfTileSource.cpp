//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: 
//
// $Id: ossimVpfTileSource.cpp 17932 2010-08-19 20:34:35Z dburken $
//----------------------------------------------------------------------------
#include <ossim/imaging/ossimVpfTileSource.h>

RTTI_DEF2(ossimVpfTileSource, "ossimVpfTileSource", ossimImageHandler, ossimViewInterface);

ossimVpfTileSource::ossimVpfTileSource()
   :
      ossimViewInterface()
{
   theObject = this;
   m_AnnotationSource = new ossimVpfAnnotationSource;
   m_AnnotationSource->setNumberOfBands(3);
}

ossimVpfTileSource::~ossimVpfTileSource()
{
   m_AnnotationSource = 0;
}

bool ossimVpfTileSource::saveState(ossimKeywordlist& kwl,
				   const char* prefix)const
{
   return m_AnnotationSource->saveState(kwl, prefix);
}

bool ossimVpfTileSource::loadState(const ossimKeywordlist& kwl,
				   const char* prefix)
{
   if (ossimImageHandler::loadState(kwl, prefix))
   {
      return m_AnnotationSource->loadState(kwl, prefix);
   }

   return false;
}

void ossimVpfTileSource::close()
{
   m_AnnotationSource->close();
}

bool ossimVpfTileSource::open()
{
   return m_AnnotationSource->open(theImageFile);
}

ossimRefPtr<ossimImageData> ossimVpfTileSource::getTile(
   const ossimIrect& tileRect, ossim_uint32 resLevel)
{
   return m_AnnotationSource->getTile(tileRect, resLevel);
}

ossim_uint32 ossimVpfTileSource::getNumberOfInputBands() const
{
   return m_AnnotationSource->getNumberOfOutputBands();
}

ossim_uint32 ossimVpfTileSource::getNumberOfOutputBands() const
{
   return m_AnnotationSource->getNumberOfOutputBands();
}

ossim_uint32 ossimVpfTileSource::getNumberOfLines(ossim_uint32 /* reduced_res_level */) const
{
   ossimIrect theBounds = m_AnnotationSource->getBoundingRect();
   
   if(theBounds.hasNans())
   {
      return theBounds.ul().x;
   }
   
   return theBounds.height();
}

ossim_uint32 ossimVpfTileSource::getNumberOfSamples(ossim_uint32 /* reduced_res_level */) const
{
   ossimIrect theBounds = m_AnnotationSource->getBoundingRect();
   
   if(theBounds.hasNans())
   {
      return theBounds.ul().x;
   }
   
   return theBounds.height();
}

ossim_uint32 ossimVpfTileSource::getNumberOfDecimationLevels() const
{
   return 32;
}

ossimIrect ossimVpfTileSource::getImageRectangle(ossim_uint32 /* reduced_res_level */) const
{
   return m_AnnotationSource->getBoundingRect();
}

//**************************************************************************************************
// Returns the image geometry object associated with this tile source or NULL if non defined.
//**************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimVpfTileSource::getImageGeometry()
{
   if (theGeometry.valid()) return theGeometry;
   
   ossimRefPtr<ossimImageGeometry> annotGeom = m_AnnotationSource->getImageGeometry();
   if ( annotGeom.valid() )
   {
      // Copy the annotation source's geometry as our own:
      theGeometry = new ossimImageGeometry(*annotGeom);

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
      
      return theGeometry;
   }
   return ossimRefPtr<ossimImageGeometry>();
}

ossimScalarType ossimVpfTileSource::getOutputScalarType() const
{
   return m_AnnotationSource->getOutputScalarType();
}

ossim_uint32 ossimVpfTileSource::getTileWidth() const
{
   return m_AnnotationSource->getTileWidth();         
}

ossim_uint32 ossimVpfTileSource::getTileHeight() const
{
   return m_AnnotationSource->getTileHeight();
}

ossim_uint32 ossimVpfTileSource::getImageTileWidth() const
{
   return 0;
}

ossim_uint32 ossimVpfTileSource::getImageTileHeight() const
{
   return 0;
}

bool ossimVpfTileSource::isOpen()const
{
   return m_AnnotationSource->isOpen();
}
   
double ossimVpfTileSource::getNullPixelValue(ossim_uint32 band)const
{
   return m_AnnotationSource->getNullPixelValue(band);
}

double ossimVpfTileSource::getMinPixelValue(ossim_uint32 band)const
{
   return m_AnnotationSource->getMinPixelValue(band);
}
      
double ossimVpfTileSource::getMaxPixelValue(ossim_uint32 band)const
{
   return m_AnnotationSource->getMaxPixelValue(band);
}

ossimObject* ossimVpfTileSource::getView()
{
   return m_AnnotationSource->getView();
}

const ossimObject* ossimVpfTileSource::getView()const
{
   return m_AnnotationSource->getView();
}
   
bool ossimVpfTileSource::setView(ossimObject*  baseObject)
{
   return m_AnnotationSource->setView(baseObject);
}

void ossimVpfTileSource::getAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList)
{
   m_AnnotationSource->getAllFeatures(featureList);
}

void ossimVpfTileSource::setAllFeatures(std::vector<ossimVpfAnnotationFeatureInfo*>& featureList)
{
   m_AnnotationSource->setAllFeatures(featureList);
}

void ossimVpfTileSource::transformObjects()
{
   m_AnnotationSource->transformObjects();
}

void ossimVpfTileSource::computeBoundingRect()
{
   m_AnnotationSource->computeBoundingRect();
}

/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageKeywordlist.h"

#include <cassert>

#include "otbMacro.h"

#include "ossim/base/ossimKeywordlist.h"
#include "ossim/base/ossimString.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "ossim/imaging/ossimImageHandlerRegistry.h"
#include "ossim/ossimTileMapModel.h"
#include "ossim/projection/ossimProjectionFactoryRegistry.h"

#include "otbSensorModelAdapter.h"

namespace otb
{

ImageKeywordlist
::ImageKeywordlist()
{
}

ImageKeywordlist
::ImageKeywordlist(const Self& p) : m_Keywordlist(p.m_Keywordlist)
{
}

ImageKeywordlist
::~ImageKeywordlist()
{
}

void
ImageKeywordlist::
operator =(const Self& p)
{
  m_Keywordlist = p.m_Keywordlist;
}

void
ImageKeywordlist::
SetKeywordlist(const ossimKeywordlist& kwl)
{
  m_Keywordlist.clear();
  for (ossimKeywordlist::KeywordMap::const_iterator it = kwl.getMap().begin();
       it != kwl.getMap().end();
       ++it)
    {
    std::string first(it->first);
    std::string second(it->second);
    m_Keywordlist[first] = second;
    }
}

const std::string&
ImageKeywordlist::
GetMetadataByKey(const std::string& key) const
{
  // Search for the key in the output map
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);

  // If the key can not be found, throw an exception
  if (it == m_Keywordlist.end())
    {
    itkExceptionMacro(<< "Keywordlist has no output with key " << key);
    }

  // Then if everything is ok, return the ossinString
  return it->second;
}

bool
ImageKeywordlist::
HasKey(const std::string& key) const
{
  KeywordlistMap::const_iterator it = m_Keywordlist.find(key);
  return (it != m_Keywordlist.end());
}

void
ImageKeywordlist::
ClearMetadataByKey(const std::string& key)
{
  m_Keywordlist[key] = "";
}

void
ImageKeywordlist::
AddKey(const std::string& key, const std::string& value)
{
  m_Keywordlist[key] = value;
}

void
ImageKeywordlist::
convertToOSSIMKeywordlist(ossimKeywordlist& kwl) const
{
  ossimKeywordlist::KeywordMap ossimMap;
  for(KeywordlistMap::const_iterator it = m_Keywordlist.begin();
      it != m_Keywordlist.end();
      ++it)
    {
    ossimMap[it->first] = it->second;
    }
  kwl.getMap() = ossimMap;
}

void
ImageKeywordlist::
Print(std::ostream& os, itk::Indent indent) const
{
  this->PrintSelf(os, indent.GetNextIndent());
}

void
ImageKeywordlist::
PrintSelf(std::ostream& os, itk::Indent indent) const
{
  ossimKeywordlist kwl;
  convertToOSSIMKeywordlist(kwl);
  os << indent << " Ossim Keyword list:" << std::endl;
  os << indent << kwl;
}

std::ostream &
operator <<(std::ostream& os, const ImageKeywordlist& kwl)
{
  kwl.Print(os);
  return os;
}

ImageKeywordlist
ReadGeometry(const std::string& filename)
{
  // Trying to read ossim MetaData
  bool             hasMetaData = false;
  ossimKeywordlist geom_kwl; // = new ossimKeywordlist();

  // Test the plugins factory
  /** Before, the pluginfactory was tested if the ossim one returned false.
      But in the case TSX, the images tif were considered as ossimQuickbirdTiffTileSource
      thus a TSX tif image wasn't read with TSX Model. We don't use the ossimRegisteryFactory
      because the default include factory contains ossimQuickbirdTiffTileSource. */
  ossimProjection * projection = ossimplugins::ossimPluginProjectionFactory::instance()
                                 ->createProjection(ossimFilename(filename.c_str()), 0);

  if (!projection)
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection FAILED ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM Instantiate projection SUCCESS ! ");

    hasMetaData = projection->saveState(geom_kwl);

    // Free memory
    delete projection;
    }

  if (!hasMetaData)
    {
    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
                                 ->open(ossimFilename(filename.c_str()));
    if (!handler)
      {
      otbMsgDevMacro(<< "OSSIM Open Image FAILED ! ");

      // In some corner cases, ossim is unable to create a ossimImageHandler
      // instance for the provided file.
      // This can happen for TIFF+geom files where the ossimTiffTileSource
      // fails when trying to open the TIFF file (for example
      // because ossim is compiled with a libtiff without BigTIFF support
      // and when the actual TIFF file exceeds the Large File limit).
      // In such cases, we don't actually need ossim to interpret the TIFF file,
      // and just want a valid ossimKeywordlist corresponding to a sensor model.

      // Here is implemented a shortcut used as a fallback scenario,
      // where we bypass the ossimImageHandlerRegistry
      // and directly create a ossimKeywordlist from the ".geom" file.

      // We then verify it is a valid sensor model by using otb::SensorModelAdapter
      // which uses ossimSensorModelFactory and ossimPluginProjectionFactory internally,
      // thus by-passing the need for a valid ossimImageHandler.

      // Try to find a ".geom" file next to 'filename'
      ossimFilename ossimGeomFile = ossimFilename(filename).setExtension(".geom");
      if (ossimGeomFile.exists() && ossimGeomFile.isFile())
        {
        // Interpret the geom file as a KWL
        ossimKeywordlist kwl(ossimGeomFile);

        // Check that the geom file results in a valid ossimKeywordlist
        if (kwl.getErrorStatus() == ossimErrorCodes::OSSIM_OK)
          {
          // Be sure there is a corresponding instance of ossimSensorModel
          // which understands this kwl
          SensorModelAdapter::Pointer sensorModel = SensorModelAdapter::New();
          ImageKeywordlist otbkwl;
          otbkwl.SetKeywordlist(kwl);
          sensorModel->CreateProjection(otbkwl);

          if (sensorModel->IsValidSensorModel())
            {
            geom_kwl = kwl;
            }
          }
        }
      }
    else
      {
      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");
      
      // Add ossimPlugins model
      ossimProjectionFactoryRegistry::instance()->registerFactory(ossimplugins::ossimPluginProjectionFactory::instance());

      //     hasMetaData = handler->getImageGeometry(geom_kwl);
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();

      if (geom.valid())
        {
        ossimProjection* projection = geom->getProjection();
        if (projection)
          {
          hasMetaData = projection->saveState(geom_kwl);
          }
        }
      }
    // Free memory
    delete handler;
    }

  if (!hasMetaData)
    {
    otbMsgDevMacro(<< "OSSIM MetaData not present ! ");
    }
  else
    {
    otbMsgDevMacro(<< "OSSIM MetaData present ! ");
    otbMsgDevMacro(<< geom_kwl);
    }

  ImageKeywordlist otb_kwl;
  otb_kwl.SetKeywordlist(geom_kwl);

  return otb_kwl;
}

void
WriteGeometry(const ImageKeywordlist& otb_kwl, const std::string& filename)
{
  // Write the image keyword list if any
  ossimKeywordlist geom_kwl;
  otb_kwl.convertToOSSIMKeywordlist(geom_kwl);

  if (geom_kwl.getSize() > 0)
    {
    otbMsgDevMacro(<< "Exporting keywordlist ...");
    ossimFilename geomFileName(filename);
    geomFileName.setExtension(".geom");
    geom_kwl.write(geomFileName.chars());
    }
}


} //namespace otb

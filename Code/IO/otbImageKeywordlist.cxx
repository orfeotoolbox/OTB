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

#include "base/ossimKeywordlist.h"
#include "base/ossimString.h"
#include "ossimTileMapModel.h"
#include "ossim/ossimPluginProjectionFactory.h"
#include "imaging/ossimImageHandlerRegistry.h"

#include "otbTileMapImageIO.h"

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
    std::string first(it->first.stringDup());
    std::string second(it->second.stringDup());
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

void
ImageKeywordlist::
ClearMetadataByKey(const std::string& key)
{
  m_Keywordlist[key] = "";
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
    ossimString first(it->first);
    ossimString second(it->second);
    ossimMap[first] = second;
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
    // Add the radar factory
    // ossimImageHandlerRegistry::instance()->addFactory(ossimImageHandlerSarFactory::instance());

    ossimImageHandler* handler = ossimImageHandlerRegistry::instance()
                                 ->open(ossimFilename(filename.c_str()));
    if (!handler)
      {
      otbMsgDevMacro(<< "OSSIM Open Image FAILED ! ");
      }

    else
      {
      otbMsgDevMacro(<< "OSSIM Open Image SUCCESS ! ");
      //     hasMetaData = handler->getImageGeometry(geom_kwl);
      ossimProjection* projection = handler->getImageGeometry()->getProjection();

      if (projection)
        {
        if (projection->getClassName() == "ossimTileMapModel")
          {
          //we need to pass the depth information which in on the IO to the projection
          //to be handle throught the kwl
          TileMapImageIO::Pointer imageIO = TileMapImageIO::New();
          imageIO->SetFileName(filename);
          // if we are here, we instanciated an ossimTileMapModel, so we should
          // be able to create a TileMapImageIO
          assert(!imageIO->CanReadFile(filename.c_str()));
          imageIO->ReadImageInformation();
          int depth = imageIO->GetDepth();
          dynamic_cast<ossimplugins::ossimTileMapModel*>(projection)->setDepth(depth);
          }
        hasMetaData = projection->saveState(geom_kwl);
//             delete projection; //FIXME find out where this should occur
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

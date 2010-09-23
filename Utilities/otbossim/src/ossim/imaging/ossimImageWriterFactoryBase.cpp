//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimImageWriterFactoryBase.cpp 18011 2010-08-31 12:48:56Z dburken $
#include <ossim/imaging/ossimImageWriterFactoryBase.h>

RTTI_DEF1(ossimImageWriterFactoryBase, "ossimImageWriterFactoryBase", ossimObjectFactory);

void ossimImageWriterFactoryBase::getImageFileWritersBySuffix(
   ossimImageWriterFactoryBase::ImageFileWriterList& /*result*/,
   const ossimString& /*ext*/)const
{
}

void ossimImageWriterFactoryBase::getImageFileWritersByMimeType(
   ossimImageWriterFactoryBase::ImageFileWriterList& /*result*/,
   const ossimString& /*mimeType*/)const
{
}

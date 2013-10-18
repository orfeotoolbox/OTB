//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimImageHandlerFactoryBase.cpp 18011 2010-08-31 12:48:56Z dburken $
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

RTTI_DEF1(ossimImageHandlerFactoryBase, "ossimImageHandlerFactoryBase", ossimObjectFactory);

void ossimImageHandlerFactoryBase::getImageHandlersBySuffix(ImageHandlerList& /*result*/,
                                                            const ossimString& /*ext*/)const
{
}

void ossimImageHandlerFactoryBase::getImageHandlersByMimeType(ImageHandlerList& /*result*/,
                                                              const ossimString& /*mimeType*/)const
{
}

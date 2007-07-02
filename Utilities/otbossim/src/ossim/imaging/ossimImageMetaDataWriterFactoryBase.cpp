//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterFactoryBase.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimImageMetaDataWriterFactoryBase.h>

RTTI_DEF1(ossimImageMetaDataWriterFactoryBase,
          "ossimImageMetaDataWriterFactoryBase",
          ossimObjectFactory);

ossimImageMetaDataWriterFactoryBase::ossimImageMetaDataWriterFactoryBase()
{
}

ossimImageMetaDataWriterFactoryBase::ossimImageMetaDataWriterFactoryBase(
   const ossimImageMetaDataWriterFactoryBase&)
{
}

const ossimImageMetaDataWriterFactoryBase&
ossimImageMetaDataWriterFactoryBase::operator=(
   const ossimImageMetaDataWriterFactoryBase&)
{
   return *this;
}

//----------------------------------------------------------------------------
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id: ossimImageMetaDataWriterFactoryBase.cpp,v 1.2 2005/06/24 17:54:09 dburken Exp $

#include  <imaging/factory/ossimImageMetaDataWriterFactoryBase.h>

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

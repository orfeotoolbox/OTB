//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description:
//
//*************************************************************************
// $Id: ossimImageDataFactory.h,v 1.9 2005/05/20 20:25:19 dburken Exp $
#ifndef ossimImageDataFactory_HEADER
#define ossimImageDataFactory_HEADER

#include <imaging/ossimImageData.h>
#include <base/common/ossimRefPtr.h>

class ossimSource;
class ossimImageSourceInterface;


/*!
 * This factory should be called by all image source producers to allocate
 * an image tile.
 */
class OSSIM_DLL ossimImageDataFactory
{
public:
   virtual ~ossimImageDataFactory();
   static ossimImageDataFactory* instance();

   virtual ossimRefPtr<ossimImageData> create(ossimSource* owner,
                                              ossimScalarType scalar,
                                              ossim_uint32 bands = 1)const;

   virtual ossimRefPtr<ossimImageData> create(ossimSource* owner,
                                              ossimScalarType scalar,
                                              ossim_uint32 bands,
                                              ossim_uint32 width,
                                              ossim_uint32 height)const;
   
   virtual ossimRefPtr<ossimImageData> create(
      ossimSource* owner,
      ossim_uint32 bands,
      ossimImageSourceInterface* inputSource)const;
   
   
   virtual ossimRefPtr<ossimImageData> create(
      ossimSource* owner,
      ossimImageSourceInterface* inputSource)const;
   
protected:
   ossimImageDataFactory(); // hide
   ossimImageDataFactory(const ossimImageDataFactory&){}//hide
   void operator = (ossimImageDataFactory&){}// hide
   
   static ossimImageDataFactory* theInstance;
};

#endif

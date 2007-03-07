//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//
//*******************************************************************
//  $Id: ossimStreamFactoryBase.h,v 1.5 2005/11/14 13:47:01 gpotts Exp $
//
#ifndef ossimStreamFactoryBase_HEADER
#define ossimStreamFactoryBase_HEADER
#include <iostream>
#include <base/common/ossimIoStream.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimRefPtr.h>
#include <base/data_types/ossimFilename.h>

class OSSIM_DLL ossimStreamFactoryBase
{
public:
   virtual ~ossimStreamFactoryBase(){}
   
   virtual ossimRefPtr<ossimIStream> createNewInputStream(
      const ossimFilename& file,
	  std::ios::openmode openMode)=0;
};

#endif

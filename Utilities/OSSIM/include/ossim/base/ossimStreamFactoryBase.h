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
//  $Id: ossimStreamFactoryBase.h 9094 2006-06-13 19:12:40Z dburken $
//
#ifndef ossimStreamFactoryBase_HEADER
#define ossimStreamFactoryBase_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>

class ossimFilename;
class ossimIStream;

class OSSIM_DLL ossimStreamFactoryBase
{
public:
   virtual ~ossimStreamFactoryBase(){}
   
   virtual ossimRefPtr<ossimIStream> createNewInputStream(
      const ossimFilename& file,
	  std::ios::openmode openMode)=0;
};

#endif

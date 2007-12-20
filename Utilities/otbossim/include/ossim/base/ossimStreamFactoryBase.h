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
//  $Id: ossimStreamFactoryBase.h 11176 2007-06-07 19:45:56Z dburken $
//
#ifndef ossimStreamFactoryBase_HEADER
#define ossimStreamFactoryBase_HEADER

#include <iosfwd>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIoStream.h>

class ossimFilename;
class ossimIStream;

class OSSIM_DLL ossimStreamFactoryBase
{
public:
   virtual ~ossimStreamFactoryBase(){}
   
   virtual ossimRefPtr<ossimIFStream> createNewIFStream(
      const ossimFilename& file,
      std::ios_base::openmode openMode)const=0;
};

#endif

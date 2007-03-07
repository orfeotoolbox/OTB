//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTag.h,v 1.8 2005/05/24 11:11:50 gpotts Exp $
#ifndef ossimNitfRegisteredTag_HEADER
#define ossimNitfRegisteredTag_HEADER
#include <iostream>
using namespace std;
#include <base/common/ossimObject.h>
#include <base/common/ossimPropertyInterface.h>

class OSSIM_DLL ossimNitfRegisteredTag : public ossimObject ,
                                         public ossimPropertyInterface
{
public:
   ossimNitfRegisteredTag(){}
   virtual ~ossimNitfRegisteredTag(){}
   
   /**
    * 
    * This will return the name of the registered tag for this
    * user defined header.
    * 
    */
   virtual ossimString getRegisterTagName()const=0;
   
   /**
    * 
    * This will allow the user defined data to parse the stream.
    * 
    */
   virtual void parseStream(std::istream& in)=0;
   virtual void writeStream(std::ostream& out)=0;

   virtual ossim_uint32 getSizeInBytes()const=0;

protected:
   ossimString theRegisteredTagName;
   
TYPE_DATA
};

#endif

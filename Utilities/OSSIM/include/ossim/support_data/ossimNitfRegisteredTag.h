//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// LICENSE:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRegisteredTag.h 10177 2007-01-03 19:22:09Z gpotts $
#ifndef ossimNitfRegisteredTag_HEADER
#define ossimNitfRegisteredTag_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPropertyInterface.h>

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

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:
   ossimString theRegisteredTagName;
   
TYPE_DATA
};

#endif

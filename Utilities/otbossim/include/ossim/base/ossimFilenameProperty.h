//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimFilenameProperty.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimFilenameProperty_HEADER
#define ossimFilenameProperty_HEADER
#include <vector>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimString.h>

class OSSIMDLLEXPORT ossimFilenameProperty : public ossimProperty
{
public:

   enum ossimFilenamePropertyIoType
   {
      ossimFilenamePropertyIoType_NOT_SPECIFIED = 0,
      ossimFilenamePropertyIoType_INPUT         = 1,
      ossimFilenamePropertyIoType_OUTPUT        = 2
   };
   
   ossimFilenameProperty(const ossimString& name = ossimString(""),
                         const ossimFilename& value = ossimFilename(""),
                         const std::vector<ossimString>& filterList = std::vector<ossimString>());

   ossimFilenameProperty(const ossimFilenameProperty& rhs);
   virtual ~ossimFilenameProperty();
   ossimObject* dup()const;
   virtual void valueToString(ossimString& valueResult)const;
  virtual bool setValue(const ossimString& value);
   const ossimFilenameProperty& operator = (ossimFilenameProperty& rhs);
   virtual const ossimProperty& assign(const ossimProperty& rhs);

   void clearFilterList();
   ossim_uint32 getNumberOfFilters()const;
   void setFilter(ossim_uint32 idx,
                  const ossimString& filterValue);
   ossimString getFilter(ossim_uint32 idx)const;
   ossimString getFilterListAsString(const ossimString& separator=ossimString("\n"))const;
   const std::vector<ossimString>& getFilterList()const;
   void addFilter(const ossimString& filter);

   void setIoType(ossimFilenamePropertyIoType ioType);
   ossimFilenamePropertyIoType getIoType()const;
   bool isIoTypeInput()const;
   bool isIoTypeOutput()const;
   
protected:
   ossimFilename            theValue;
   std::vector<ossimString> theFilterList;
   ossimFilenamePropertyIoType theIoType;
   
TYPE_DATA
};

#endif

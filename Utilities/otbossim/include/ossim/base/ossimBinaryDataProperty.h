//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
//*************************************************************************

#ifndef ossimBinaryDataProperty_HEADER
#define ossimBinaryDataProperty_HEADER

#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimRtti.h>

#include <vector>

class OSSIMDLLEXPORT ossimBinaryDataProperty : public ossimProperty
{
public:
   ossimBinaryDataProperty(const ossimString& name=ossimString(""),
      const std::vector<unsigned char>& binaryData = std::vector<unsigned char>());

   virtual ~ossimBinaryDataProperty();

   virtual bool setValue(const ossimString& value);

   virtual void valueToString(ossimString& valueResult) const;

   void setBinaryData(const vector<unsigned char>& binaryData);

   const vector<unsigned char>& getBinaryData() const;

private:
   std::vector<unsigned char> theBinaryData;

TYPE_DATA
};

#endif
